#include "tools/utils/file.hpp"

namespace tools::file {

std::string read_all_text(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file {}" + path);
    }

    std::string content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(content.data(), content.size());
    return content;
}

} // namespace tools::file