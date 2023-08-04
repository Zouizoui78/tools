#include "tools/waveform/WaveformGenerator.hpp"
#include "tools/waveform/Constants.hpp"

namespace tools::waveform {

bool WaveformGenerator::add_waveform(std::shared_ptr<AWaveform> sound) {
    std::lock_guard lock(_mutex);
    auto it = std::find(_waveforms.begin(), _waveforms.end(), sound);
    if (it != _waveforms.end())
        return false;
    _waveforms.push_back(sound);
    return true;
}

bool WaveformGenerator::remove_waveform(std::shared_ptr<AWaveform> sound) {
    std::lock_guard lock(_mutex);
    auto it = std::find(_waveforms.begin(), _waveforms.end(), sound);
    if (it == _waveforms.end())
        return false;
    _waveforms.erase(it);
    return true;
}

double WaveformGenerator::generate_sample() {
    double ret = 0;
    double time = static_cast<double>(_sample_n) / sampling_rate;
    {
        std::lock_guard lock(_mutex);
        for (auto sound : _waveforms) {
            ret += sound->synthesize(WaveformTimepoint{ time, _sample_n });
        }
    }
    _sample_n++;
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

void WaveformGenerator::reset_samples() {
    _sample_n = 0;
}

} // namespace tools::waveform
