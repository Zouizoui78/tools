#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <atomic>
#include <curl/curl.h>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>

namespace tools::net {

class HTTPClient;

/**
 * @brief HTTP reply content packaging class.
 *
 */
class HTTPReply {
    public:
    /**
     * @brief Default constructor.
     */
    HTTPReply();

    /**
     * @brief Construct an HTTPReply object.
     */
    HTTPReply(CURL *handle);

    /**
     * @brief Destructor.
     */
    ~HTTPReply();

    /**
     * @brief Restore the object attributes to their default values :
     * res_code = 0 ;
     * data = "" ;
     * host = "";
     * _handle = nullptr
     */
    void reset();

    long res_code;
    std::string data;
    std::string host;

    private:
    CURL *_handle;

    // Grant access to _handle to HTTPClient without providing getters.
    friend class HTTPClient;
};

using Callback = std::function<void (HTTPReply &)>;

/**
 * @brief Very simple cURL wrapper for performing HTTP GET and POST requests.
 *
 */
class HTTPClient {
    public:

    /**
     * @brief Default constructor.
     */
    HTTPClient();

    /**
     * @brief Destructor.
     */
    ~HTTPClient();

    /**
     * @brief Tell whether the object has successfully initialized or not.
     * @return true if curl initialization was successful ; false otherwise
     */
    bool is_ok();

    /**
     * @brief Perform a blocking HTTP POST request.
     *
     * @param reply OUTPUT Reply object. Its fields are cleared before the request if performed.
     * @param url URL to post to.
     * @param headers headers to send with the request.
     * @return bool Ok or not.
     */
    bool get(HTTPReply &reply, const std::string &url, const std::vector<std::string> &headers = {});

    /**
     * @brief Perform a blocking HTTP POST request.
     *
     * @param reply OUTPUT Reply object.
     * @param url URL to post to.
     * @param post_data Data to send with POST requests.
     * @param headers Headers to add to the request.
     * @return bool Ok or not.
     */
    bool post(
        HTTPReply &reply,
        const std::string &url,
        const std::string &post_data,
        const std::vector<std::string> &headers = {}
    );

    /**
     * @brief Perform a non-blocking HTTP GET request.
     *
     * @param url URL to post to.
     * @param callback Called after the request is complete.
     * @param headers Headers to send with the request.
     * @return bool Ok or not.
     */
    bool get_async(
        const std::string &url,
        Callback callback,
        const std::vector<std::string> &headers = {}
    );

    /**
     * @brief Perform a non-blocking HTTP POST request.
     *
     * @param url URL to post to.
     * @param callback Called after the request is complete.
     * @param headers Headers to send with the request.
     * @return bool Ok or not.
     */
    bool post_async(
        const std::string &url,
        const std::string &post_data,
        Callback callback,
        const std::vector<std::string> &headers = {}
    );

    /**
     * @brief Return when all pending async requests are done.
     */
    void wait_async_requests();

    private:

    /**
     * @brief Callback called by curl when requests are done.
     *
     * @param data Data received. Does not necesserally contains the entire response,
     * i.e. this callback can be called several times by curl.
     * @param size Always 1 according to curl docs.
     * @param nmemb Size of data.
     * @param instance Custom object passed by the user. We pass the reply object through it.
     * @return size_t Size of received data.
     */
    static size_t curl_callback(char *data, size_t size, size_t nmemb, void *instance);

    /**
     * @brief Initialize an easy handle.
     *
     * @return CURL* Created handle ; nullptr if failed.
     */
    CURL *easy_handle_init();

    /**
     * @brief Set the handle's headers.
     *
     * @param handle Handle to modify.
     * @param headers Strings containing the headers to add to the list.
     * @return curl_slist * Headers list. The caller should free it when the request is done. nullptr if empty headers vector.
     */
    curl_slist *set_easy_handle_headers(CURL *handle, const std::vector<std::string> &headers);

    /**
     * @brief Set the common easy handle options that
     * are needed by both sync and async requests.
     *
     * @param handle curl handle to modify.
     * @param url Request URL.
     * @param post_data POST request data. If empty the handle is set to send a GET request.
     * @param reply Passed to curl callback.
     * @return Ok or not.
     */
    bool set_easy_handle_opt(
        CURL *handle,
        const std::string &url,
        const std::string &post_data,
        const HTTPReply &reply
    );

    /**
     * @brief Join and remove terminated threads stored in _threads_map.
     */
    void cleanup_threads();

    /**
     * @brief Parse the url to extract the host.
     * E.g. if url == "https://example.com:12345/test", it returns "example.com".
     *
     * @param url URL to parse.
     * @return std::string Host.
     */
    std::string get_host_from_url(const std::string &url);

    /**
     * @brief Internal method used to send synchronous requests with _sync_handle.
     *
     * @param reply Reply object.
     * @param url URL to send request to.
     * @param headers Headers to apply on the request.
     * @param post_data Data to send with POST requests. If empty a GET request is performed.
     * @return Ok or not.
     */
    bool sync_request(
        HTTPReply &reply,
        const std::string &url,
        const std::vector<std::string> &headers = {},
        const std::string &post_data = ""
    );

    /**
     * @brief Internal method used to send asynchronous requests.
     *
     * @param url URL to send request to.
     * @param callback Called when responses is received.
     * @param headers Headers to apply on the request.
     * @param post_data Data to send with POST requests. If empty a GET request is performed.
     * @return Ok or not.
     */
    bool async_request (
        const std::string &url,
        Callback callback,
        const std::vector<std::string> &headers = {},
        const std::string &post_data = ""
    );

    // These three attributes are used to handle curl initialisation and cleanup.
    static std::atomic<uint32_t> _instance_count;
    static std::atomic<bool> _global_curl_initialized;
    std::mutex _instances_mutex;

    // Attributes used during async requests to keep track of running threads
    // and cleaned the terminated ones.
    std::map<std::thread::id, std::thread> _threads_map;
    std::vector<std::thread::id> _threads_to_join;
    std::mutex _threads_map_mutex;
    std::mutex _threads_to_join_mutex;
};

} // namespace tools::net

#endif // HTTPCLIENT_HPP