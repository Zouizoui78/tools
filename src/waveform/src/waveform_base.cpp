#include "tools/waveform/waveform_base.hpp"
#include "tools/waveform/constants.hpp"

#include <algorithm>

namespace tools::waveform {

WaveformBase::WaveformBase() {
    set_volume(1);
    set_frequency(440);
}

double WaveformBase::get_volume() const {
    return _volume / constants::volume_mult;
}

void WaveformBase::set_volume(double volume) {
    _volume = std::clamp(volume, 0.0, 1.0) * constants::volume_mult;
}

double WaveformBase::get_frequency() const {
    return _frequency;
}

void WaveformBase::set_frequency(double frequency) {
    _frequency = frequency;
    _period = 1.0 / frequency;
    _samples_in_period = constants::sampling_rate / _frequency;
}

double WaveformBase::get_period() const {
    return _period;
}

int WaveformBase::get_samples_in_period() const {
    return _samples_in_period;
}

} // namespace tools::waveform
