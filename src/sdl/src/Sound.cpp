#include "sdl/Sound.hpp"

#include "spdlog/spdlog.h"

#include <numbers>

namespace tools::sdl {

constexpr int32_t SOUND_SAMPLING_RATE = 44100;
constexpr double VOLUME_MULT = 0.01;

// Default waveform implementations
ASound::ASound() {
    set_volume(1);
    set_frequency(440);
}

double ASound::get_volume() const {
    return _volume / VOLUME_MULT;
}

void ASound::set_volume(double volume) {
    _volume = volume * VOLUME_MULT;
}

double ASound::get_frequency() const  {
    return _frequency;
}

void ASound::set_frequency(double frequency) {
    if (frequency <= 0)
        return;
    _frequency = frequency;
    _period = 1.0 / frequency;
    _sampling_period = SOUND_SAMPLING_RATE / _frequency;
}

double ASound::get_period() const {
    return _period;
}

int16_t ASound::get_sampling_period() const {
    return _sampling_period;
}


Sinus::Sinus() : ASound() {
    update_angular_freq();
}

double Sinus::synthesize(SynthesisTimepoint timepoint) const {
    return _volume * sin(_angular_freq * timepoint.time);
}

void Sinus::set_frequency(double frequency) {
    ASound::set_frequency(frequency);
    update_angular_freq();
}

double Sinus::get_angular_freq() const {
    return _angular_freq;
}

void Sinus::update_angular_freq() {
    _angular_freq = 2.0 * std::numbers::pi * _frequency;
}


Square::Square() : ASound() {
    update_sampling_duty_cycle();
}

double Square::synthesize(SynthesisTimepoint timepoint) const {
    return (timepoint.sample_n % _sampling_period) >= _sampling_duty_cycle ? -_volume : _volume;
}

void Square::set_frequency(double frequency) {
    ASound::set_frequency(frequency);
    update_sampling_duty_cycle();
}

double Square::get_duty_cycle() const {
    return _duty_cycle;
}

void Square::set_duty_cycle(double duty_cycle) {
    _duty_cycle = duty_cycle;
    update_sampling_duty_cycle();
}

int16_t Square::get_sampling_duty_cycle() const {
    return _sampling_duty_cycle;
}

void Square::update_sampling_duty_cycle() {
    _sampling_duty_cycle = _sampling_period * _duty_cycle;
}

/////////////////////////////////////


// SoundPlayer

SoundPlayer::SoundPlayer() {
#ifdef WINDOWS
    putenv("SDL_AUDIODRIVER=dsound");
#endif
    init();
}

SoundPlayer::~SoundPlayer() {
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

bool SoundPlayer::init() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        spdlog::error("Failed to initialize audio subsystem : {}", SDL_GetError());
        _is_audio_initialized = false;
    }

    SDL_AudioSpec desired;
    desired.freq = SOUND_SAMPLING_RATE;
    desired.format = AUDIO_F32SYS;
    desired.channels = 1;
    desired.samples = 512; // samples/s
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

bool SoundPlayer::is_initialized() {
    return _is_audio_initialized;
}

bool SoundPlayer::add_sound(std::shared_ptr<ASound> sound) {
    auto it = std::find(_sounds.begin(), _sounds.end(), sound);
    if (it != _sounds.end())
        return false;
    _sounds.push_back(sound);
    return true;
}

bool SoundPlayer::remove_sound(std::shared_ptr<ASound> sound) {
    auto it = std::find(_sounds.begin(), _sounds.end(), sound);
    if (it == _sounds.end())
        return false;
    _sounds.erase(it);
    return true;
}

double SoundPlayer::synthesize() {
    double ret = 0;
    double time = static_cast<double>(_sample_n) / SOUND_SAMPLING_RATE;
    for (auto sound : _sounds) {
        ret += sound->synthesize(SynthesisTimepoint{ time, _sample_n });
    }
    _sample_n++;
    return ret;
}

std::vector<double> SoundPlayer::synthesize_n_samples(int n_samples) {
    std::vector<double> ret;
    ret.reserve(n_samples);
    for (int i = 0 ; i < n_samples ; i++) {
        ret.push_back(synthesize());
    }
    return ret;
}

void SoundPlayer::sdl_callback(void *instance, uint8_t *raw_buffer, int bytes) {
    SoundPlayer *player = static_cast<SoundPlayer *>(instance);
    float *buffer = reinterpret_cast<float *>(raw_buffer);

    int32_t len = bytes / sizeof(float);
    for (int32_t i = 0 ; i < len ; i++) {
        buffer[i] = static_cast<float>(player->synthesize());
    }
}

void SoundPlayer::play() {
    SDL_PauseAudioDevice(_audio_device_id, 0);
}

void SoundPlayer::pause() {
    SDL_PauseAudioDevice(_audio_device_id, 1);
}

} // namespace tools::sdl