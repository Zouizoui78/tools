#include "tools/utils/Files.hpp"
#include "tools/utils/Log.hpp"

namespace tools::utils::files {

std::shared_ptr<spdlog::logger> logger = new_logger("Files");

std::string read_text_file(const std::string &path) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::stringstream content;
        content << file.rdbuf();
        return content.str();
    }
    else {
        logger->error("Failed to open file {}.", path);
        return "";
    }
}

std::vector<uint8_t> read_binary_file(const std::string &path) {
    std::vector<uint8_t> result;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        logger->error("Failed to open file {}.", path);
        return result;
    }

    auto len = std::filesystem::file_size(path);
    uint8_t block_size = sizeof(uint8_t);
    size_t block_n = len / block_size;

    uint8_t tmp;
    for (size_t i = 0 ; i < block_n ; ++i) {
        file.read((char *)&tmp, block_size);
        result.push_back(tmp);
    }

    return result;
}

bool write_binary_file(const std::vector<uint8_t> &data, const std::string &path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        logger->error("Failed to open file {}.", path);
        return false;
    }

    uint8_t block_size = sizeof(uint8_t);
    for (size_t i = 0 ; i < data.size() ; ++i) {
        file.write((char *)&data[i], block_size);
    }

    return true;
}

} // namespace tools::utils