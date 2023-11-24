#include "tools/sdl/sdl_audio_device.hpp"
#include "tools/waveform/constants.hpp"

#include <stdexcept>
#include <utility>

namespace tools::sdl {

SDLAudioDevice::~SDLAudioDevice() {
    if (_audio_device_id != 0) {
        SDL_CloseAudioDevice(_audio_device_id);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}

SDLAudioDevice::SDLAudioDevice(SDLAudioDevice&& other)
    : _audio_device_id(std::exchange(_audio_device_id, 0)) {}

SDLAudioDevice& SDLAudioDevice::operator=(SDLAudioDevice&& other) {
    std::swap(_audio_device_id, other._audio_device_id);
    return *this;
}

SDLAudioDevice::SDLAudioDevice(SDL_AudioCallback audio_callback) {
#ifdef WIN32
    putenv("SDL_AUDIODRIVER=dsound");
#endif
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_AudioSpec desired;
    desired.freq = tools::waveform::constants::sampling_rate;
    desired.format = AUDIO_F32SYS;
    desired.channels = 1;
    desired.samples = 512;
    desired.callback =
        audio_callback; // called periodically by SDL to refill the buffer
    desired.userdata = this;

    _audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &desired, nullptr, 0);

    if (_audio_device_id == 0) {
        throw std::runtime_error(SDL_GetError());
    }
}

SDL_AudioDeviceID SDLAudioDevice::get_device_id() const {
    return _audio_device_id;
}

} // namespace tools::sdl