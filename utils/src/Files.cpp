#include "tools/utils/Files.hpp"

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

} // namespace tools::utils