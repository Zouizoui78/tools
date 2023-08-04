#include "waveform/AWaveform.hpp"
#include "waveform/Constants.hpp"

#include <algorithm>

namespace tools::waveform {

AWaveform::AWaveform() {
    set_volume(1);
    set_frequency(440);
}

double AWaveform::get_volume() const {
    return _volume / volume_mult;
}

void AWaveform::set_volume(double volume) {
    _volume = std::clamp(volume, 0.0, 1.0) * volume_mult;
}

double AWaveform::get_frequency() const  {
    return _frequency;
}

void AWaveform::set_frequency(double frequency) {
    if (frequency < 1e-9)
        return;
    _frequency = frequency;
    _period = 1.0 / frequency;
    _samples_in_period = sampling_rate / _frequency;
}

double AWaveform::get_period() const {
    return _period;
}

int16_t AWaveform::get_samples_in_period() const {
    return _samples_in_period;
}

} // namespace tools::waveform
