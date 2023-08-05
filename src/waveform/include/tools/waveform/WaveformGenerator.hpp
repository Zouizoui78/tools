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
    bool add_waveform(std::shared_ptr<AWaveform> waveform);

    // Return false if waveform not found.
    bool remove_waveform(std::shared_ptr<AWaveform> waveform);

    // Every time this function is called,
    // the returned sample is the next one in the waveform.
    double generate_sample();

    std::vector<double> generate_n_samples(int n_samples);

    // Set _sample_n to 0 to reset the synthesis time.
    void reset_samples();

private:
    std::vector<std::shared_ptr<AWaveform>> _waveforms;
    std::mutex _mutex;

    // Index of the next sample generated by generate_sample.
    std::atomic<int64_t> _sample_n = 0;
};

} // namespace tools::waveform

#endif // WAVEFORM_GENERATOR_HPP
