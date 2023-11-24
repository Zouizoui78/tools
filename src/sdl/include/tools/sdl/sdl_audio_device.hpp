#ifndef SDL_AUDIO_DEVICE_HPP
#define SDL_AUDIO_DEVICE_HPP

#include "SDL2/SDL.h"

namespace tools::sdl {

class SDLAudioDevice {
private:
    SDL_AudioDeviceID _audio_device_id = 0;

public:
    SDLAudioDevice(SDL_AudioCallback audio_callback);

    ~SDLAudioDevice() noexcept;
    SDLAudioDevice(const SDLAudioDevice& other) = delete;
    SDLAudioDevice(SDLAudioDevice&& other);
    SDLAudioDevice& operator=(const SDLAudioDevice& other) = delete;
    SDLAudioDevice& operator=(SDLAudioDevice&& other);

    SDL_AudioDeviceID get_device_id() const;
};

} // namespace tools::sdl

#endif // SDL_AUDIO_DEVICE_HPP