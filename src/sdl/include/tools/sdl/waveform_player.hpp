#ifndef WAVEFORM_PLAYER_HPP
#define WAVEFORM_PLAYER_HPP

#include "SDL2/SDL.h"
#include "tools/waveform/waveform_generator.hpp"
#include <memory>

namespace tools::sdl {

class WaveformPlayer {
public:
    ~WaveformPlayer();

    WaveformPlayer();
    WaveformPlayer(const WaveformPlayer& other) = delete;
    WaveformPlayer(WaveformPlayer&& other) = default;
    WaveformPlayer& operator=(const WaveformPlayer& other) = delete;
    WaveformPlayer& operator=(WaveformPlayer&& other) = default;

    // Return false if waveform is already registered.
    // Do not take ownership.
    bool add_waveform(const tools::waveform::WaveformBase* waveform);

    // Return false if waveform not found.
    bool remove_waveform(const tools::waveform::WaveformBase* waveform);

    void play() const;
    void pause() const;
    bool is_paused() const;

private:
    SDL_AudioDeviceID _audio_device_id = 0;
    tools::waveform::WaveformGenerator _generator;

    void init();
    static void sdl_callback(void* instance, uint8_t* raw_buffer, int bytes);
};

} // namespace tools::sdl

#endif // WAVEFORM_PLAYER_HPP
