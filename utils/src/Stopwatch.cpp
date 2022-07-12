#include "tools/utils/Stopwatch.hpp"
#include "tools/utils/Log.hpp"

namespace tools::utils {

static auto logger = new_logger("Stopwatch");

Stopwatch::Stopwatch(const std::string &name) {
    _name = name;
    _start_time_point = std::chrono::steady_clock::now();
}
    
Stopwatch::~Stopwatch() {
    log(get_duration());
}

double Stopwatch::get_duration() const {
    return (std::chrono::steady_clock::now() - _start_time_point).count();
}

void Stopwatch::log(double d) const {
    std::string prefix = "";
    if (!_name.empty())
        prefix = _name + " : ";

    std::string unit = " ns";

    if (d > 1e9) {
        d /= 1e9;
        unit = " s";
    }
    else if (d > 1e6) {
        d /= 1e6;
        unit = " ms";
    }
    else if (d > 1e3) {
        d /= 1e3;
        unit = " us";
    }
    
    logger->info("{}{:.3f}{}", prefix, d, unit);
}

} // namespace tools::utils