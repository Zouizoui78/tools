#ifndef FILE_HPP
#define FILE_HPP

#include <algorithm>
#include <fstream>
#include <ranges>
#include <vector>

namespace tools::file {

std::string read_all_text(const std::string& path);

template <typename T>
std::vector<T> read_all_binary(const std::string& path) {
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
    file.read(reinterpret_cast<char*>(content.data()), file_size);

    return content;
}

template <typename T>
concept non_contiguous = !std::ranges::contiguous_range<T>;

// Dump the data from the range into a binary file.
// Data is copied by chunk into a temporary chunk_size-long vector
// to reduce the numbers of write calls.
// Return number of written bytes.
template <typename R>
requires non_contiguous<R>
int dump_range(const std::string& path, R&& range,
               int64_t chunk_size = 1000000) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw new std::runtime_error("Failed to open file {}" + path);
    }

    using T = std::ranges::range_value_t<R>;
    auto type_size = sizeof(T);

    auto write_chunk = [&file,
                        &type_size](std::ranges::viewable_range auto&& chunk) {
        std::vector<T> vec(chunk.begin(), chunk.end());
        file.write(reinterpret_cast<char*>(vec.data()),
                   chunk.size() * type_size);
    };

    for (size_t i = 0; i < range.size();) {
        if (i + chunk_size > range.size()) {
            write_chunk(std::ranges::subrange(range.begin() + i, range.end()));
        } else {
            write_chunk(std::views::counted(range.begin() + i, chunk_size));
        }
        i += chunk_size;
    }
    // std::ranges::for_each(range | std::views::counted(), write_chunk);
    return range.size() * type_size;
}

// Dump the contiguous data from the range into a binary file.
// Return number of written bytes.
template <typename R>
requires std::ranges::contiguous_range<R>
int dump_range(const std::string& path, R&& range) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw new std::runtime_error("Failed to open file {}" + path);
    }

    auto total_size = range.size() * sizeof(std::ranges::range_value_t<R>);
    file.write(reinterpret_cast<char*>(range.data()), total_size);
    return total_size;
}

} // namespace tools::file

#endif // FILE_HPP