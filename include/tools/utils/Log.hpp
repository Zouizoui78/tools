#ifndef LOG_HPP
#define LOG_HPP

#define SPDLOG_FMT_EXTERNAL
#define SPDLOG_COMPILED_LIB
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace tools::utils {

std::shared_ptr<spdlog::logger> new_logger(const std::string &logger_name);

} // namespace tools::utils

#endif // LOG_HPP