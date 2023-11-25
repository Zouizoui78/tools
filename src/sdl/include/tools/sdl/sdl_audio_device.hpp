#ifndef SDL_AUDIO_DEVICE_HPP
#define SDL_AUDIO_DEVICE_HPP

#include "SDL2/SDL.h"
#include <functional>

namespace tools::sdl {

class SDLAudioDevice {
private:
    SDL_AudioDeviceID _audio_device_id = 0;
    std::function<void(uint8_t* raw_buffer, int len)> _sdl_callback;

    static void sdl_callback(void* raw_instance, uint8_t* raw_buffer, int len);

public:
    SDLAudioDevice(
        std::function<void(uint8_t* raw_buffer, int len)> sdl_callback);

    ~SDLAudioDevice() noexcept;
    SDLAudioDevice(const SDLAudioDevice& other) = delete;
    SDLAudioDevice(SDLAudioDevice&& other) = delete;
    SDLAudioDevice& operator=(const SDLAudioDevice& other) = delete;
    SDLAudioDevice& operator=(SDLAudioDevice&& other) = delete;

    SDL_AudioDeviceID get_device_id() const;
};

} // namespace tools::sdl

#endif // SDL_AUDIO_DEVICE_HPP