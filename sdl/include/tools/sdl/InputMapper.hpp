#ifndef INPUTMAPPER_HPP
#define INPUTMAPPER_HPP

#include "nlohmann/json.hpp"
#include "SDL2/SDL_keyboard.h"

namespace tools::sdl {

using nlohmann::json;

class InputMapper {
    public:
    
    InputMapper();

    ~InputMapper();

    bool add_mapping(SDL_Keycode key, uint8_t mapped_key);
    bool add_mapping(const std::string &key, uint8_t mapped_key);

    bool remove_mapping(SDL_Keycode key);
    bool remove_mapping(const std::string &key);

    bool load_map(const json &map);
    bool load_map(const std::string &json_path);

    int map_key(SDL_Keycode key);
    int map_key(const std::string &key);

    private:

    std::map<SDL_Keycode, uint8_t> _keymap;
};

} // namespace tools::sdl

#endif // INPUTMAPPER_HPP