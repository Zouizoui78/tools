#ifndef WAVEFORM_BASE_HPP
#define WAVEFORM_BASE_HPP

#include "constants.hpp"
#include "waveform_timepoint.hpp"

namespace tools::waveform {

class WaveformBase {
public:
    virtual ~WaveformBase() noexcept = default;

    WaveformBase(const WaveformBase &other) = delete;
    WaveformBase(WaveformBase &&other) = delete;
    WaveformBase &operator=(const WaveformBase &other) = delete;
    WaveformBase &operator=(WaveformBase &&other) = delete;

    WaveformBase() = default;

    virtual double synthesize(WaveformTimepoint timepoint) const = 0;

    double get_volume() const;

    // Clamp volume to [0, 1]
    void set_volume(double volume);

    double get_frequency() const;

    virtual void set_frequency(double frequency);

    double get_period() const;
    int get_samples_in_period() const;

protected:
    double _volume = constants::volume_mult;
    double _frequency = 1;
    double _period = 1;
    int _samples_in_period = constants::sampling_rate;
};

} // namespace tools::waveform

#endif // WAVEFORM_BASE_HPP
