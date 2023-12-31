#ifndef INPUTMAPPER_HPP
#define INPUTMAPPER_HPP

#include "SDL2/SDL_keyboard.h"
#include <map>
#include <string>

namespace tools::sdl {

/**
 * @brief Class allowing to assign a SDL keycode to a value.
 */
class InputMapper {
public:
    InputMapper();
    ~InputMapper();

    /**
     * @brief Set a mapping. If a mapping for the same key exists, it is
     * overwritten.
     *
     * @param key SDL Keycode to map.
     * @param mapped_value Value mapped to the key.
     */
    void set_mapping(SDL_Keycode key, uint8_t mapped_value);

    /**
     * @brief Set a mapping. If a mapping for the same key exists, it is
     * overwritten.
     *
     * The name of the key must match one of the entry from this page :
     * https://wiki.libsdl.org/SDL_Keycode
     *
     * @param key Name of the key to map. Must match an SDL_Keycode name.
     * @param mapped_value Value mapped to the key.
     * @return true ; no error
     * @return false ; key name doesn't exist
     */
    bool set_mapping(const std::string& key, uint8_t mapped_value);

    /**
     * @brief Remove the mapping of the given key.
     *
     * @param key
     */
    void remove_mapping(SDL_Keycode key);

    /**
     * @brief Remove the mapping of the named key.
     *
     * The name of the key must match one of the entry from this page :
     * https://wiki.libsdl.org/SDL_Keycode
     *
     * @param key
     * @return true ; no error
     * @return false ; key name doesn't exist
     */
    bool remove_mapping(const std::string& key);

    /**
     * @brief Return the value mapped to the passed key.
     *
     * @param key
     * @return int
     */
    int map_key(SDL_Keycode key);

    /**
     * @brief Return the value mapped to the passed key.
     *
     * @param key
     * @return int
     */
    int map_key(const std::string& key);

private:
    std::map<SDL_Keycode, uint8_t> _keymap;
};

} // namespace tools::sdl

#endif // INPUTMAPPER_HPP