#include "tools/net/HTTPClient.hpp"
#include "tools/utils/Log.hpp"

namespace tools::net {

std::atomic<uint32_t> HTTPClient::_instance_count = 0;
std::atomic<bool> HTTPClient::_global_curl_initialized = false;

HTTPClient::HTTPClient() {
    SPDLOG_DEBUG("HTTPClient ctor");

    {
        std::lock_guard<std::mutex> lock(_instances_mutex);
        if (!_global_curl_initialized) {
            SPDLOG_INFO("Curl global initialization");
            if(curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
                SPDLOG_ERROR("Failed to perform curl global initialization.");
            else
                _global_curl_initialized = true;
        }
    }

    ++_instance_count;
    SPDLOG_DEBUG("HTTPClient instances : {}", _instance_count);
}

HTTPClient::~HTTPClient() {
    SPDLOG_DEBUG("HTTPClient dtor");

    if (!_threads_to_join.empty()) {
        cleanup_threads();
    }

    if (!_threads_map.empty()) {
        SPDLOG_INFO("Waiting for pending async requests.");
        wait_async_requests();
    }

    {
        std::lock_guard<std::mutex> lock(_instances_mutex);
        --_instance_count;
        SPDLOG_DEBUG("Remaining HTTPClient instances : {}", _instance_count);
        if (_instance_count == 0 && _global_curl_initialized) {
            SPDLOG_INFO("Curl global cleanup");
            curl_global_cleanup();
            _global_curl_initialized = false;
        }
    }
}

bool HTTPClient::is_ok() {
    return _global_curl_initialized;
}

bool HTTPClient::get(HTTPReply &reply, const std::string &url, const std::vector<std::string> &headers) {
    SPDLOG_DEBUG("Synchronous GET request to \"{}\"", url);
    return sync_request(reply, url, headers, "");
}

bool HTTPClient::post(
    HTTPReply &reply,
    const std::string &url,
    const std::string &post_data,
    const std::vector<std::string> &headers
) {
    SPDLOG_DEBUG("Synchronous POST request to \"{}\" with data : {}", url, post_data);
    if (post_data.empty()) {
        SPDLOG_ERROR("Cannot perform POST request, POST data is empty.");
        return false;
    }
    return sync_request(reply, url, headers, post_data);
}

bool HTTPClient::get_async(
    const std::string &url,
    Callback callback,
    const std::vector<std::string> &headers
) {
    SPDLOG_DEBUG("Asynchronous GET request to \"{}\"", url);
    return async_request(url, callback, headers);
}

bool HTTPClient::post_async(
    const std::string &url,
    const std::string &post_data,
    Callback callback,
    const std::vector<std::string> &headers
) {
    SPDLOG_DEBUG("Asynchronous POST request to \"{}\" with data : {}", url, post_data);
    if (post_data.empty()) {
        SPDLOG_ERROR("Cannot perform POST request to \"{}\", POST data is empty.", url);
        return false;
    }
    return async_request(url, callback, headers, post_data);
}


bool HTTPClient::sync_request(
    HTTPReply &reply,
    const std::string &url,
    const std::vector<std::string> &headers,
    const std::string &post_data
) {
    std::string host = get_host_from_url(url);

    if (url.empty()) {
        SPDLOG_ERROR("Cannot perform request, url is empty.");
        return false;
    }

    CURL *handle = easy_handle_init();
    if (!handle) {
        SPDLOG_ERROR("Aborting request to {} : failed to initialize easy handle.", host);
        return false;
    }

    reply.reset();
    reply.host = host;
    reply._handle = handle;

    if (!set_easy_handle_opt(handle, url, post_data, reply)
    ) {
        SPDLOG_ERROR("Aborting request to {} : failed to set easy handle options.", host);
        curl_easy_cleanup(handle);
        return false;
    }

    curl_slist *headers_list = nullptr;
    if (!headers.empty()) {
        headers_list = set_easy_handle_headers(handle, headers);
        if (!headers_list) {
            SPDLOG_ERROR("Aborting request to {} : failed to set headers.", host);
            curl_easy_cleanup(handle);
            return false;
        }
    }

    CURLcode res = curl_easy_perform(handle);
    if (res) {
        SPDLOG_ERROR("Failed request to {} : {}", host, curl_easy_strerror(res));
        return false;
    }

    if (headers_list != nullptr) {
        curl_slist_free_all(headers_list);
        headers_list = nullptr;
    }

    curl_easy_cleanup(handle);

    return true;
}

bool HTTPClient::async_request (
    const std::string &url,
    Callback callback,
    const std::vector<std::string> &headers,
    const std::string &post_data
) {
    cleanup_threads();

    std::string host = get_host_from_url(url);

    if (url.empty()) {
        SPDLOG_ERROR("Cannot perform asynchronous request, url is empty.");
        return false;
    }

    if (!callback) {
        SPDLOG_ERROR("Cannot perform asynchronous request to {}, callback is invalid.", host);
        return false;
    }

    std::thread thread([this, host, url, callback, headers, post_data]() {
        HTTPReply reply;

        // Here we simply make a synchronous request in the new thread.
        bool res = sync_request(reply, url, headers, post_data);

        if (res)
            callback(reply);

        // Mark this thread as done.
        {
            std::lock_guard<std::mutex> lock(_threads_to_join_mutex);
            _threads_to_join.push_back(std::this_thread::get_id());
        }
    });

    // Add the new thread to the tracked threads map.
    {
        std::lock_guard<std::mutex> lock(_threads_map_mutex);
        _threads_map[thread.get_id()] = std::move(thread);
    }

    return true;
}

CURL *HTTPClient::easy_handle_init() {
    CURL *handle = curl_easy_init();
    if (!handle) {
        SPDLOG_ERROR("Failed to initialize curl easy handle.");
        return nullptr;
    }

    CURLcode res = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, HTTPClient::curl_callback);
    if (res) {
        SPDLOG_ERROR("Failed to set easy handle callback.");
        return nullptr;
    }

    #ifdef WINDOWS
    res = curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, false);
    if (res)
        SPDLOG_WARN("Failed to disable SSL verification.");
    #endif

    return handle;
}

curl_slist *HTTPClient::set_easy_handle_headers(CURL *handle, const std::vector<std::string> &headers) {
    curl_slist *headers_list = nullptr;
    for (auto const &e : headers) {
        headers_list = curl_slist_append(headers_list, e.c_str());
        if (headers_list == nullptr) {
            SPDLOG_WARN("Failed to set header : {}", e);
            return nullptr;
        }
        SPDLOG_DEBUG("Added header : \"{}\"", e);
    }

    if (curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers_list)) {
        SPDLOG_WARN("Failed to attach header list to easy handle.");
        curl_slist_free_all(headers_list);
        return nullptr;
    }

    return headers_list;
}

bool HTTPClient::set_easy_handle_opt(
    CURL *handle,
    const std::string &url,
    const std::string &post_data,
    const HTTPReply &reply
) {
    CURLcode res = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    if (res) {
        SPDLOG_ERROR("Failed to set easy handle URL.");
        return false;
    }

    res = curl_easy_setopt(handle, CURLOPT_WRITEDATA, static_cast<const void *>(&reply));
    if (res) {
        SPDLOG_ERROR("Failed to set easy handle callback object.");
        return false;
    }

    if (post_data.empty()) {
        // If post_data is empty, we assume we are sending a GET request.
        // So we disable POST mode which might have been enabled
        // by a previous POST request.
        res = curl_easy_setopt(handle, CURLOPT_POST, 0);
        if (res) {
            SPDLOG_ERROR("Failed to disable curl POST mode.");
            return false;
        }
    }
    else {
        // This implicitly set CURLOPT_POST to 1, activating POST mode.
        res = curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_data.c_str());
        if (res) {
            SPDLOG_ERROR("Failed to set POST fields.");
            return false;
        }
    }

    return true;
}

size_t HTTPClient::curl_callback(char *data, size_t size, size_t nmemb, void *instance) {
    if (!instance) {
        SPDLOG_ERROR("Aborting curl callback : invalid instance ptr.");
        return 0;
    }

    HTTPReply *reply = reinterpret_cast<HTTPReply *>(instance);
    if (!reply) {
        SPDLOG_ERROR("Aborting curl callback : invalid casted reply ptr.");
        return 0;
    }

    // Curl examples do it like this, so...
    size_t realsize = size * nmemb;

    // Get response data.
    if (data != nullptr && nmemb > 0) {
        SPDLOG_DEBUG("curl callback : received {} bytes from {}", realsize, reply->host);
        reply->data += data;
    }

    // Get HTTP response code.
    if (curl_easy_getinfo(reply->_handle, CURLINFO_RESPONSE_CODE, &reply->res_code)) {
        SPDLOG_WARN("Failed to get HTTP response code.");
        reply->res_code = 0;
    }

    return realsize;
}

void HTTPClient::cleanup_threads() {
    std::scoped_lock lock(_threads_map_mutex, _threads_to_join_mutex);
    if (_threads_to_join.size() > 0) {
        SPDLOG_DEBUG("Cleaning {} threads.", _threads_to_join.size());
        for (const auto &e : _threads_to_join) {
            _threads_map[e].join();
            _threads_map.erase(e);
        }
        _threads_to_join.clear();
    }
}

void HTTPClient::wait_async_requests() {
    {
        std::lock_guard<std::mutex> lock(_threads_map_mutex);
        for (auto &e : _threads_map) {
            e.second.join();
        }
        _threads_map.clear();
    }
    {
        std::lock_guard<std::mutex> lock(_threads_to_join_mutex);
        _threads_to_join.clear();
    }
}

std::string HTTPClient::get_host_from_url(const std::string &url) {
    std::size_t double_slash = url.find("//");

    std::size_t host_begin = 0;
    if (double_slash == std::string::npos)
        host_begin = 0;
    else
        host_begin = double_slash + 2;

    std::size_t colon = url.find(":");
    if (colon == std::string::npos)
        return url.substr(host_begin, url.find("/", host_begin) - host_begin);
    else
        return url.substr(host_begin, colon - host_begin);
}


// HTTPReply

HTTPReply::HTTPReply() {
    reset();
}

HTTPReply::HTTPReply(CURL *handle) {
    reset();
    _handle = handle;
}

HTTPReply::~HTTPReply() {}

void HTTPReply::reset() {
    res_code = 0;
    data = "";
    host = "";
    _handle = nullptr;
}

} // namespace tools::net