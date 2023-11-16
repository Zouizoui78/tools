#include "tools/utils/stopwatch.hpp"

namespace tools::utils {

using namespace std::chrono;

Stopwatch::Stopwatch() : _start_time_point(steady_clock::now()) {}

void Stopwatch::lap() {
    _laps.emplace_back(std::chrono::steady_clock::now());
}

void Stopwatch::reset() {
    _laps.clear();
    _start_time_point = steady_clock::now();
}

} // namespace tools::utils