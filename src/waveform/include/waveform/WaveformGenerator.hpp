#ifndef WAVEFORM_GENERATOR_HPP
#define WAVEFORM_GENERATOR_HPP

#include <memory>
#include <mutex>
#include <vector>

#include "AWaveform.hpp"

namespace tools::waveform {

class WaveformGenerator {
public:
    // Return false if waveform is already registered.
    bool add_waveform(std::shared_ptr<AWaveform> sound);

    // Return false if waveform not found.
    bool remove_waveform(std::shared_ptr<AWaveform> sound);

    // Every time this function is called,
    // the returned sample is the next one in the waveform.
    double generate_sample();

    std::vector<double> generate_n_samples(int n_samples);

private:
    std::vector<std::shared_ptr<AWaveform>> _sounds;
    int64_t _sample_n = 0;
    std::mutex _mutex;
};

} // namespace tools::waveform

#endif // WAVEFORM_GENERATOR_HPP
