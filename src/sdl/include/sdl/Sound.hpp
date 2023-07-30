#ifndef SOUND_HPP
#define SOUND_HPP

#include <cstdint>
#include <vector>

#include <SDL2/SDL.h>

namespace tools::sdl {

struct SoundSynthesisData {
    uint32_t sample_n;
    double time;
};

class ASound {
    public:

    ASound();

    virtual double synthesize(SoundSynthesisData data) const = 0;

    double get_volume() const;
    virtual void set_volume(double volume);

    double get_frequency() const;
    virtual void set_frequency (double frequency);

    double get_period() const;

    protected:

    double _volume;

    double _frequency;
    double _period;
    uint32_t _sampling_period;
};

class Sinus : public ASound {
    public:

    Sinus();

    virtual double synthesize(SoundSynthesisData data) const override;

    virtual void set_frequency(double frequency) override;

    private:

    void update_freq_mult();

    double _freq_mult = 0;
};

class Square : public ASound {
    public:

    Square();

    virtual double synthesize(SoundSynthesisData data) const override;

    virtual void set_frequency(double frequency) override;

    double get_duty_cycle() const;
    void set_duty_cycle(double duty_cycle);

    private:

    void update_sampling_duty_cycle();

    double _duty_cycle = 0.5;
    uint32_t _sampling_duty_cycle = 1;
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
    bool add_sound(ASound *sound);

    /**
     * @brief Remove a sound from the registered sounds vector.
     *
     * @param sound Sound to remove.
     * @return true No error.
     * @return false Sound not found in vector.
     */
    bool remove_sound(ASound *sound);

    void play();
    void pause();

    double make_sample();
    std::vector<double> make_samples(int n_samples);

    private:

    static void sdl_callback(void *instance, uint8_t *raw_buffer, int bytes);

    bool init();

    std::vector<ASound *> _sounds;

    uint32_t _sample_n = 0;
    bool _is_audio_initialized = false;
};

} // namespace tools::sdl

#endif // SOUND_HPP