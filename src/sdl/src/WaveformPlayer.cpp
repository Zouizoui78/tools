#include "sdl/WaveformPlayer.hpp"
#include "waveform/Constants.hpp"

#include "spdlog/spdlog.h"

namespace tools::sdl {

WaveformPlayer::WaveformPlayer(std::shared_ptr<tools::waveform::WaveformGenerator> generator) : _generator(generator) {
#ifdef WINDOWS
    putenv("SDL_AUDIODRIVER=dsound");
#endif
    init();
}

WaveformPlayer::~WaveformPlayer() {
    if (_audio_device_id != 0) {
        SDL_CloseAudioDevice(_audio_device_id);
        _audio_device_id = 0;
    }

    if (is_initialized()) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        _is_audio_initialized = false;
        spdlog::info("SDL audio subsystem cleaned up.");
    }
}

bool WaveformPlayer::init() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        spdlog::error("Failed to initialize audio subsystem : {}", SDL_GetError());
        _is_audio_initialized = false;
    }

    SDL_AudioSpec desired;
    desired.freq = tools::waveform::sampling_rate;
    desired.format = AUDIO_F32SYS;
    desired.channels = 1;
    desired.samples = 512;
    desired.callback = sdl_callback; // called periodically by SDL to refill the buffer
    desired.userdata = this;

    _audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &desired, nullptr, 0);

    if (_audio_device_id == 0) {
        spdlog::error("Failed to open sound device : {}", SDL_GetError());
        _is_audio_initialized = false;
    }

    spdlog::info("SDL audio subsystem initialized.");
    _is_audio_initialized = true;
    return _is_audio_initialized;
}

bool WaveformPlayer::is_initialized() const {
    return _is_audio_initialized;
}

void WaveformPlayer::sdl_callback(void *instance, uint8_t *raw_buffer, int bytes) {
    WaveformPlayer *player = static_cast<WaveformPlayer *>(instance);
    float *buffer = reinterpret_cast<float *>(raw_buffer);

    int32_t len = bytes / sizeof(float);
    for (int32_t i = 0 ; i < len ; i++) {
        buffer[i] = static_cast<float>(player->_generator->generate_sample());
    }
}

void WaveformPlayer::play() const {
    SDL_PauseAudioDevice(_audio_device_id, 0);
}

void WaveformPlayer::pause() const {
    SDL_PauseAudioDevice(_audio_device_id, 1);
}

} // namespace tools::sdl
