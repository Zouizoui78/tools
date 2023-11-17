#include "tools/waveform/square.hpp"

#include <algorithm>

namespace tools::waveform {

Square::Square() {
    set_duty_cycle(0.5);
}

double Square::synthesize(WaveformTimepoint timepoint) const {
    return (timepoint.sample_n % _samples_in_period) >= _sampling_duty_cycle
               ? -_volume
               : _volume;
}

void Square::set_frequency(double frequency) {
    WaveformBase::set_frequency(frequency);
    update_sampling_duty_cycle();
}

double Square::get_duty_cycle() const {
    return _duty_cycle;
}

void Square::set_duty_cycle(double duty_cycle) {
    _duty_cycle = std::clamp(duty_cycle, 0.01, 0.99);
    update_sampling_duty_cycle();
}

int Square::get_sampling_duty_cycle() const {
    return _sampling_duty_cycle;
}

void Square::update_sampling_duty_cycle() {
    _sampling_duty_cycle = _samples_in_period * _duty_cycle;
}

} // namespace tools::waveform
