#include "tools/waveform/waveform_generator.hpp"
#include "tools/waveform/constants.hpp"

#include <algorithm>

namespace tools::waveform {

bool WaveformGenerator::add_waveform(std::shared_ptr<AWaveform> waveform) {
    std::lock_guard lock(_mutex);
    auto it = std::find(_waveforms.begin(), _waveforms.end(), waveform);
    if (it != _waveforms.end())
        return false;
    _waveforms.push_back(waveform);
    return true;
}

bool WaveformGenerator::remove_waveform(std::shared_ptr<AWaveform> waveform) {
    std::lock_guard lock(_mutex);
    auto it = std::find(_waveforms.begin(), _waveforms.end(), waveform);
    if (it == _waveforms.end())
        return false;
    _waveforms.erase(it);
    return true;
}

double WaveformGenerator::generate_sample() {
    double ret = 0;
    double time = static_cast<double>(_sample_index) / sampling_rate;
    {
        std::lock_guard lock(_mutex);
        for (auto waveform : _waveforms) {
            ret += waveform->synthesize(WaveformTimepoint{ time, _sample_index });
        }
    }
    _sample_index++;
    return ret;
}

std::vector<double> WaveformGenerator::generate_n_samples(int n_samples) {
    std::vector<double> ret;
    ret.reserve(n_samples);
    for (int i = 0 ; i < n_samples ; i++) {
        ret.push_back(generate_sample());
    }
    return ret;
}

int64_t WaveformGenerator::get_sample_index() const {
    return _sample_index;
}

void WaveformGenerator::set_sample_index(int64_t sample_index) {
    _sample_index = sample_index;
}

void WaveformGenerator::reset_sample_index() {
    set_sample_index(0);
}

} // namespace tools::waveform
