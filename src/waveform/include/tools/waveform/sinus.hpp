#ifndef SINUS_HPP
#define SINUS_HPP

#include "waveform_base.hpp"

namespace tools::waveform {

class Sinus : public WaveformBase {
public:
    Sinus();
    double synthesize(WaveformTimepoint timepoint) const override;
    void set_frequency(double frequency) override;
    double get_angular_freq() const;

private:
    double _angular_freq = 0;

    void update_angular_freq();
};

} // namespace tools::waveform

#endif // SINUS_HPP
