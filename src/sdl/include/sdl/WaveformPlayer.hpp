#ifndef WAVEFORM_PLAYER_HPP
#define WAVEFORM_PLAYER_HPP

#include <memory>
#include "SDL2/SDL.h"
#include "waveform/WaveformGenerator.hpp"

namespace tools::sdl {

class WaveformPlayer {
public:
    ~WaveformPlayer();
    WaveformPlayer(std::shared_ptr<tools::waveform::WaveformGenerator> generator);

    bool is_initialized() const;

    void play() const;
    void pause() const;

private:
    SDL_AudioDeviceID _audio_device_id = 0;
    bool _is_audio_initialized = false;
    std::shared_ptr<tools::waveform::WaveformGenerator> _generator;

    bool init();
    static void sdl_callback(void *instance, uint8_t *raw_buffer, int bytes);
};

} // namespace tools::sdl

#endif // WAVEFORM_PLAYER_HPP
