#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>
#include <vector>

#include "endian.hpp"

namespace tools::file {

// Read a file and return its content as a string.
std::string read_all_text(const std::string &path);

// Read a binary file and copy its content into the returned vector.
template <typename T>
std::vector<T> read_all_binary(const std::string &path, std::endian endianness = std::endian::native) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw new std::runtime_error("Failed to open file {}" + path);
    }

    std::vector<T> content;
    file.seekg(0, std::ios::end);

    auto file_size = file.tellg();
    auto type_size = sizeof(T);

    content.resize(file_size / type_size);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(content.data()), file_size);

    if (endianness != std::endian::native) {
        for (auto &element : content) {
            element = tools::endian::swap_bytes(element);
        }
    }

    return content;
}

// Dump the data between the iterators into a binary file.
// Return number of written bytes.
template <typename T, typename Iter>
int write_binary(const std::string &path, Iter begin, Iter end, std::endian endianness = std::endian::native) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw new std::runtime_error("Failed to open file {}" + path);
    }

    auto data_size = std::distance(begin, end);
    auto type_size = sizeof(T);
    bool need_byte_swap = endianness != std::endian::native;

    while (begin != end) {
        auto tmp = *begin;
        if (need_byte_swap) {
            tmp = tools::endian::swap_bytes(tmp);
        }
        file.write(reinterpret_cast<char *>(&tmp), type_size);
        begin++;
    }

    return data_size * type_size;
}

} // namespace tools::file

#endif // FILE_HPP