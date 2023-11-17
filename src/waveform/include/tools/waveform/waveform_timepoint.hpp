#ifndef WAVEFORM_TIMEPOINT_HPP
#define WAVEFORM_TIMEPOINT_HPP

#include <cstdint>

namespace tools::waveform {

struct WaveformTimepoint {
    // Synthesis time in seconds since the beginning of the synthesis
    double time;

    // Index of the sample starting from 0 at the beginning of the synthesis
    int sample_n;
};

} // namespace tools::waveform

#endif // WAVEFORM_TIMEPOINT_HPP
