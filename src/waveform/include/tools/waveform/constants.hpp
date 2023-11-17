#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace tools::waveform::constants {

constexpr int sampling_rate = 44100;
constexpr double nyquist_frequency = sampling_rate / 2.0;
constexpr double volume_mult = 0.01;

} // namespace tools::waveform::constants

#endif // CONSTANTS_HPP
