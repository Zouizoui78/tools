#ifndef FILE_HPP
#define FILE_HPP

#include <algorithm>
#include <fstream>
#include <ranges>
#include <vector>

namespace tools::file {

// Read a file and return its content as a string.
std::string read_all_text(const std::string &path);

// Read a binary file and copy its content into the returned vector.
template <typename T>
std::vector<T> read_all_binary(const std::string &path) {
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

    return content;
}

// Dump the data in the range into a binary file.
// Return number of written bytes.
template <typename R>
requires std::ranges::input_range<R>
int dump_range(const std::string &path, R&& range) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw new std::runtime_error("Failed to open file {}" + path);
    }

    using T = std::ranges::range_value_t<R>;
    auto type_size = sizeof(T);

    auto write_chunk = [&file, &type_size](std::ranges::viewable_range auto&& chunk) {
        std::vector<T> vec(chunk.begin(), chunk.end());
        file.write(reinterpret_cast<char *>(vec.data()), chunk.size() * type_size);
    };

    // Write data by chunk of 1M elements
    std::ranges::for_each(range | std::views::chunk(1000000), write_chunk);
    return range.size() * type_size;
}

// Dump the contiguous data in the range into a binary file.
// Return number of written bytes.
template <typename R>
requires std::ranges::contiguous_range<R>
int dump_range(const std::string &path, R&& range) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw new std::runtime_error("Failed to open file {}" + path);
    }

    auto total_size = range.size() * sizeof(std::ranges::range_value_t<R>);
    file.write(reinterpret_cast<char *>(range.data()), total_size);
    return total_size;
}

} // namespace tools::file

#endif // FILE_HPP