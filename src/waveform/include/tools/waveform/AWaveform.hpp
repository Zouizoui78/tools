#ifndef AWAVEFORM_HPP
#define AWAVEFORM_HPP

#include "waveform_timepoint.hpp"

namespace tools::waveform {

class AWaveform {
public:
    AWaveform();

    virtual double synthesize(WaveformTimepoint timepoint) const = 0;

    double get_volume() const;

    // Clamp volume to [0, 1]
    virtual void set_volume(double volume);

    double get_frequency() const;

    // Does nothing if frequency < 1e-9
    virtual void set_frequency (double frequency);

    double get_period() const;
    int16_t get_samples_in_period() const;

protected:
    double _volume;
    double _frequency;
    double _period;
    int16_t _samples_in_period;
};

} // namespace tools::waveform

#endif // AWAVEFORM_HPP
