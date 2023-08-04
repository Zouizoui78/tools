#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "AWaveform.hpp"

namespace tools::waveform {

class Square : public AWaveform {
public:
    Square();

    virtual double synthesize(WaveformTimepoint timepoint) const override;
    virtual void set_frequency(double frequency) override;

    double get_duty_cycle() const;

    // Clamp volume to [0.01, 0.99]
    void set_duty_cycle(double duty_cycle);

    int16_t get_sampling_duty_cycle() const;

private:
    double _duty_cycle;
    int16_t _sampling_duty_cycle;

    void update_sampling_duty_cycle();
};

} // namespace tools::waveform

#endif // SQUARE_HPP
