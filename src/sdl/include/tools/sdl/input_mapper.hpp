#ifndef INPUTMAPPER_HPP
#define INPUTMAPPER_HPP

#include "SDL2/SDL_keyboard.h"
#include <map>
#include <string>

namespace tools::sdl {

class InputMapper {
public:
    InputMapper();
    ~InputMapper();

    // Already existing mappings are overwritten.
    void set_mapping(SDL_Keycode key, int mapped_value);

    // Already existing mappings are overwritten.
    // The name of the key must match one of the entry from this page :
    // https://wiki.libsdl.org/SDL_Keycode
    // Return false if the passed key name is not recognized.
    bool set_mapping(const std::string& key_name, int mapped_value);

    void remove_mapping(SDL_Keycode key);

    // The name of the key must match one of the entry from this page :
    // https://wiki.libsdl.org/SDL_Keycode
    // Return false if the passed key name is not recognized.
    bool remove_mapping(const std::string& key_name);

    int map_key(SDL_Keycode key);
    int map_key(const std::string& key_name);

private:
    std::map<SDL_Keycode, int> _keymap;
};

} // namespace tools::sdl

#endif // INPUTMAPPER_HPP