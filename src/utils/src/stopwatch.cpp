#include "tools/utils/stopwatch.hpp"

namespace tools::utils {

using namespace std::chrono;

Stopwatch::Stopwatch() {
    reset();
}

void Stopwatch::reset() {
    _start_time_point = steady_clock::now();
}

} // namespace tools::utils