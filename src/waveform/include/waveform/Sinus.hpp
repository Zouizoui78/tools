#ifndef SINUS_HPP
#define SINUS_HPP

#include "AWaveform.hpp"

namespace tools::waveform {

class Sinus : public AWaveform {
public:
    Sinus();
    virtual double synthesize(WaveformTimepoint timepoint) const override;
    virtual void set_frequency(double frequency) override;
    double get_angular_freq() const;

private:
    double _angular_freq = 0;

    void update_angular_freq();
};

} // namespace tools::waveform

#endif // SINUS_HPP
