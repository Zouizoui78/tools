#include "waveform/Square.hpp"

namespace tools::waveform {

Square::Square() : AWaveform() {
    update_sampling_duty_cycle();
}

double Square::synthesize(WaveformTimepoint timepoint) const {
    return (timepoint.sample_n % _sampling_period) >= _sampling_duty_cycle
            ? -_volume
            : _volume;
}

void Square::set_frequency(double frequency) {
    AWaveform::set_frequency(frequency);
    update_sampling_duty_cycle();
}

double Square::get_duty_cycle() const {
    return _duty_cycle;
}

void Square::set_duty_cycle(double duty_cycle) {
    _duty_cycle = duty_cycle;
    update_sampling_duty_cycle();
}

int16_t Square::get_sampling_duty_cycle() const {
    return _sampling_duty_cycle;
}

void Square::update_sampling_duty_cycle() {
    _sampling_duty_cycle = _sampling_period * _duty_cycle;
}

} // namespace tools::waveform
