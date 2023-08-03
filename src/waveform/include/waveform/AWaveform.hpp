#ifndef AWAVEFORM_HPP
#define AWAVEFORM_HPP

#include "WaveformTimepoint.hpp"

namespace tools::waveform {

class AWaveform {
public:
    AWaveform();

    virtual double synthesize(WaveformTimepoint timepoint) const = 0;

    double get_volume() const;
    virtual void set_volume(double volume);

    double get_frequency() const;
    virtual void set_frequency (double frequency);

    double get_period() const;
    int16_t get_sampling_period() const;

protected:
    double _volume;
    double _frequency;
    double _period;
    int16_t _sampling_period;
};

} // namespace tools::waveform

#endif // AWAVEFORM_HPP
