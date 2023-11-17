#include "tools/sdl/waveform_player.hpp"
#include "tools/waveform/constants.hpp"

namespace tools::sdl {

WaveformPlayer::~WaveformPlayer() {
    if (_audio_device_id != 0) {
        SDL_CloseAudioDevice(_audio_device_id);
        _audio_device_id = 0;
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

WaveformPlayer::WaveformPlayer() {
#ifdef WIN32
    putenv("SDL_AUDIODRIVER=dsound");
#endif
    init();
}

bool WaveformPlayer::add_waveform(
    const tools::waveform::WaveformBase* waveform) {
    return _generator.add_waveform(waveform);
}

bool WaveformPlayer::remove_waveform(
    const tools::waveform::WaveformBase* waveform) {
    return _generator.remove_waveform(waveform);
}

void WaveformPlayer::play() const {
    SDL_PauseAudioDevice(_audio_device_id, 0);
}

void WaveformPlayer::pause() const {
    SDL_PauseAudioDevice(_audio_device_id, 1);
}

bool WaveformPlayer::is_paused() const {
    return SDL_GetAudioDeviceStatus(_audio_device_id) == SDL_AUDIO_PAUSED;
}

void WaveformPlayer::init() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_AudioSpec desired;
    desired.freq = tools::waveform::constants::sampling_rate;
    desired.format = AUDIO_F32SYS;
    desired.channels = 1;
    desired.samples = 512;
    desired.callback =
        sdl_callback; // called periodically by SDL to refill the buffer
    desired.userdata = this;

    _audio_device_id = SDL_OpenAudioDevice(nullptr, 0, &desired, nullptr, 0);

    if (_audio_device_id == 0) {
        throw std::runtime_error(SDL_GetError());
    }
}

void WaveformPlayer::sdl_callback(void* instance, uint8_t* raw_buffer,
                                  int bytes) {
    WaveformPlayer* player = static_cast<WaveformPlayer*>(instance);
    float* buffer = reinterpret_cast<float*>(raw_buffer);

    int len = bytes / sizeof(float);
    for (int i = 0; i < len; i++) {
        buffer[i] = static_cast<float>(player->_generator.generate_sample());
    }
}

} // namespace tools::sdl
