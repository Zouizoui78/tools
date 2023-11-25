#ifndef WAVEFORM_PLAYER_HPP
#define WAVEFORM_PLAYER_HPP

#include "sdl_audio_device.hpp"
#include "tools/waveform/waveform_generator.hpp"
#include <memory>

namespace tools::sdl {

class WaveformPlayer {
public:
    WaveformPlayer();

    // Return false if waveform is already registered.
    // Do not take ownership.
    bool add_waveform(const tools::waveform::WaveformBase* waveform);

    // Return false if waveform not found.
    bool remove_waveform(const tools::waveform::WaveformBase* waveform);

    void play() const;
    void pause() const;
    bool is_paused() const;

private:
    void sdl_callback(uint8_t* raw_buffer, int len);

    std::unique_ptr<SDLAudioDevice> _audio_device;
    tools::waveform::WaveformGenerator _generator;
};

} // namespace tools::sdl

#endif // WAVEFORM_PLAYER_HPP
