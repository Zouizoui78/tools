#include "tools/waveform/Sinus.hpp"

#include <cmath>
#include <numbers>

namespace tools::waveform {

Sinus::Sinus() : AWaveform() {
    update_angular_freq();
}

double Sinus::synthesize(WaveformTimepoint timepoint) const {
    return _volume * sin(_angular_freq * timepoint.time);
}

void Sinus::set_frequency(double frequency) {
    AWaveform::set_frequency(frequency);
    update_angular_freq();
}

double Sinus::get_angular_freq() const {
    return _angular_freq;
}

void Sinus::update_angular_freq() {
    _angular_freq = 2.0 * std::numbers::pi * _frequency;
}

} // namespace tools::waveform
