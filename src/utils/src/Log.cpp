#include "tools/utils/Log.hpp"

namespace tools::utils {

static std::shared_ptr<spdlog::logger> _logger;
static bool _init_done = false;

std::shared_ptr<spdlog::logger> new_logger(const std::string &logger_name) {
    // Initialize a dummy logger from which future loggers will be cloned.
    if (!_init_done) {
        char *logdir = std::getenv("TEST_OUTPUTS");
        std::string logfile = "/log.txt";
        if (!logdir)
            logfile = "logs" + logfile;
        else
            logfile = logdir + logfile;

        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(logfile, 0, 0));

        _logger = std::make_shared<spdlog::logger>("base_logger", sinks.begin(), sinks.end());

        // [YYY-MM-DD hh:mm:ss.µs] [thread id] [name] [level] msg
        // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
        _logger->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%t] [%n] [%^%l%$] %v");

        #ifdef DEBUG
        _logger->set_level(spdlog::level::debug);
        #endif

        _init_done = true;
        _logger->debug("Initialized logging.");
    }
    
    auto logger = _logger->clone(logger_name);
    return logger;
}

} // namespace tools::utils