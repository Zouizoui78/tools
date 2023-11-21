#ifndef WAVEFORM_BASE_HPP
#define WAVEFORM_BASE_HPP

#include "waveform_timepoint.hpp"

namespace tools::waveform {

class WaveformBase {
public:
    virtual ~WaveformBase() noexcept = default;

    WaveformBase(const WaveformBase& other) = default;
    WaveformBase(WaveformBase&& other) = default;
    WaveformBase& operator=(const WaveformBase& other) = default;
    WaveformBase& operator=(WaveformBase&& other) = default;

    WaveformBase();

    virtual double synthesize(WaveformTimepoint timepoint) const = 0;

    double get_volume() const;

    // Clamp volume to [0, 1]
    virtual void set_volume(double volume);

    double get_frequency() const;

    virtual void set_frequency(double frequency);

    double get_period() const;
    int get_samples_in_period() const;

protected:
    double _volume;
    double _frequency;
    double _period;
    int _samples_in_period;
};

} // namespace tools::waveform

#endif // WAVEFORM_BASE_HPP
