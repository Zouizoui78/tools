#include "tools/utils/file.hpp"
#include <format>
#include <iostream>

namespace tools::file {

std::string read_text_file(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error(std::format("Failed to open file {}", path));
    }

    std::string content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(content.data(), content.size());
    return content;
}

std::vector<uint8_t> read_binary_file(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw new std::runtime_error(std::format("Failed to open file {}", path));
    }

    std::vector<uint8_t> content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(content.data()), content.size());
    return content;
}

bool write_binary_file(const std::vector<uint8_t> &data, const std::string &path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast<const char *>(data.data()), data.size());
    return true;
}

} // namespace tools::utils::files