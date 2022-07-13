#ifndef FILES_HPP
#define FILES_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "tools/utils/Log.hpp"

namespace tools::utils::files {

extern std::shared_ptr<spdlog::logger> logger;

/**
 * @brief Read a file and return its content as a string.
 * @param path Path of the file to read.
 * @return String filled with the content of the file.
 */
std::string read_text_file(const std::string &path);

/**
 * @brief Read a binary file into the returned vector.
 * @param path Path of the file to read.
 * @return vector containing the read bytes.
 */
template<typename T>
std::vector<T> read_binary_file(const std::string &path) {
    std::vector<T> result;

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        logger->error("Failed to open file '{}'.", path);
        return result;
    }

    auto len = std::filesystem::file_size(path);
    uint8_t block_size = sizeof(T);
    size_t block_n = len / block_size;

    T tmp;
    for (size_t i = 0 ; i < block_n ; ++i) {
        file.read((char *)&tmp, block_size);
        result.push_back(tmp);
    }

    return result;
}

/**
 * @brief Write the content of the given vector into a binary file.
 * @param vector Vector to dump.
 * @param path Path of the file to write.
 * @return true => ok ; false => error.
 */
template<typename T>
bool write_binary_file(const std::vector<T> &data, const std::string &path) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        logger->error("Failed to open file '{}'.", path);
        return false;
    }

    uint8_t block_size = sizeof(T);
    for (size_t i = 0 ; i < data.size() ; ++i) {
        file.write((char *)&data[i], block_size);
    }

    return true;
}

} // namespace tools::utils

#endif // FILES_HPP