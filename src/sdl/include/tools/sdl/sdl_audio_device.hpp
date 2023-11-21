#ifndef SDL_AUDIO_DEVICE_HPP
#define SDL_AUDIO_DEVICE_HPP

#include "SDL2/SDL.h"

namespace tools::sdl {

class SDLAudioDevice {
public:
    ~SDLAudioDevice() noexcept;

    SDLAudioDevice(const SDLAudioDevice& other) = delete;
    SDLAudioDevice(SDLAudioDevice&& other) = default;
    SDLAudioDevice& operator=(const SDLAudioDevice& other) = delete;
    SDLAudioDevice& operator=(SDLAudioDevice&& other) = default;

    SDLAudioDevice(SDL_AudioCallback audio_callback);

    SDL_AudioDeviceID get_device_id() const;

private:
    SDL_AudioDeviceID _audio_device_id = 0;
};

} // namespace tools::sdl

#endif // SDL_AUDIO_DEVICE_HPP