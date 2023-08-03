#ifndef SOUND_HPP
#define SOUND_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>

namespace tools::sdl {

struct SynthesisTimepoint {
    // Synthesis time in seconds since the beginning of the synthesis
    double time;

    // Index of the sample starting from 0 at the beginning of the synthesis
    int64_t sample_n;
};

class ASound {
    public:

    ASound();

    virtual double synthesize(SynthesisTimepoint timepoint) const = 0;

    double get_volume() const;
    virtual void set_volume(double volume);

    double get_frequency() const;
    virtual void set_frequency (double frequency);

    double get_period() const;
    int16_t get_sampling_period() const;

    protected:

    double _volume;

    double _frequency;
    double _period;
    int16_t _sampling_period;
};

class Sinus : public ASound {
    public:

    Sinus();

    virtual double synthesize(SynthesisTimepoint timepoint) const override;

    virtual void set_frequency(double frequency) override;

    double get_angular_freq() const;

    private:

    void update_angular_freq();

    double _angular_freq = 0;
};

class Square : public ASound {
    public:

    Square();

    virtual double synthesize(SynthesisTimepoint timepoint) const override;

    virtual void set_frequency(double frequency) override;

    double get_duty_cycle() const;
    void set_duty_cycle(double duty_cycle);

    int16_t get_sampling_duty_cycle() const;

    private:

    void update_sampling_duty_cycle();

    double _duty_cycle = 0.5;
    int16_t _sampling_duty_cycle = 1;
};

class SoundPlayer {
    public:

    SoundPlayer();
    ~SoundPlayer();

    bool is_initialized();

    /**
     * @brief Add a sound to be played when play() is called.
     *
     * @param sound sound to register.
     * @return true No error.
     * @return false Sound already registered.
     */
    bool add_sound(std::shared_ptr<ASound> sound);

    /**
     * @brief Remove a sound from the registered sounds vector.
     *
     * @param sound Sound to remove.
     * @return true No error.
     * @return false Sound not found in vector.
     */
    bool remove_sound(std::shared_ptr<ASound> sound);

    void play();
    void pause();

    double synthesize();
    std::vector<double> synthesize_n_samples(int n_samples);

    private:

    static void sdl_callback(void *instance, uint8_t *raw_buffer, int bytes);

    bool init();

    std::vector<std::shared_ptr<ASound>> _sounds;

    SDL_AudioDeviceID _audio_device_id = 0;
    int64_t _sample_n = 0;
    bool _is_audio_initialized = false;
};

} // namespace tools::sdl

#endif // SOUND_HPP