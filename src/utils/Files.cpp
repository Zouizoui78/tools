#include "tools/utils/Files.hpp"
#include "tools/utils/Log.hpp"

namespace tools::utils::files {

std::string read_text_file(const std::string &path) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::stringstream content;
        content << file.rdbuf();
        std::string ret = content.str();
        if (ret.back() == '\n')
            ret.pop_back();
        return ret;
    }
    else {
        SPDLOG_ERROR("Failed to open file {} : {}", path, strerror(errno));
        return "";
    }
}

std::vector<uint8_t> read_binary_file(const std::string &path) {
    std::vector<uint8_t> result;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file {} : {}", path, strerror(errno));
        return result;
    }

    auto len = std::filesystem::file_size(path);
    uint8_t block_size = sizeof(uint8_t);
    size_t block_n = len / block_size;
    SPDLOG_DEBUG("Reading {} bytes from '{}'", block_n, path);

    uint8_t tmp;
    for (size_t i = 0 ; i < block_n ; ++i) {
        file.read(reinterpret_cast<char *>(&tmp), block_size);
        result.push_back(tmp);
    }

    return result;
}

bool write_binary_file(const std::vector<uint8_t> &data, const std::string &path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file {} : {}", path, strerror(errno));
        return false;
    }

    uint8_t block_size = sizeof(uint8_t);
    for (size_t i = 0 ; i < data.size() ; ++i) {
        file.write(reinterpret_cast<const char *>(&data[i]), block_size);
    }

    return true;
}

} // namespace tools::utils