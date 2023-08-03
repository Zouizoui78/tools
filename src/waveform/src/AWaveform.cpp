#include "waveform/AWaveform.hpp"
#include "waveform/Constants.hpp"

namespace tools::waveform {

AWaveform::AWaveform() {
    set_volume(1);
    set_frequency(440);
}

double AWaveform::get_volume() const {
    return _volume / volume_mult;
}

void AWaveform::set_volume(double volume) {
    _volume = volume * volume_mult;
}

double AWaveform::get_frequency() const  {
    return _frequency;
}

void AWaveform::set_frequency(double frequency) {
    if (frequency <= 0)
        return;
    _frequency = frequency;
    _period = 1.0 / frequency;
    _sampling_period = sampling_rate / _frequency;
}

double AWaveform::get_period() const {
    return _period;
}

int16_t AWaveform::get_sampling_period() const {
    return _sampling_period;
}

} // namespace tools::waveform
