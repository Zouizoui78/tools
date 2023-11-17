#include "tools/sdl/input_mapper.hpp"
#include "tools/utils/file.hpp"

namespace tools::sdl {

InputMapper::InputMapper() {}
InputMapper::~InputMapper() {}

void InputMapper::set_mapping(SDL_Keycode key, int mapped_key) {
    _keymap[key] = mapped_key;
}

bool InputMapper::set_mapping(const std::string& key_name, int mapped_key) {
    SDL_Keycode sdl_key = SDL_GetKeyFromName(key_name.c_str());
    if (sdl_key == SDLK_UNKNOWN) {
        return false;
    }
    set_mapping(sdl_key, mapped_key);
    return true;
}

void InputMapper::remove_mapping(SDL_Keycode key) {
    _keymap.erase(key);
}

bool InputMapper::remove_mapping(const std::string& key_name) {
    SDL_Keycode sdl_key = SDL_GetKeyFromName(key_name.c_str());
    if (sdl_key == SDLK_UNKNOWN) {
        return false;
    }
    remove_mapping(sdl_key);
    return true;
}

int InputMapper::map_key(SDL_Keycode key) {
    std::map<SDL_Keycode, int>::iterator value_it = _keymap.find(key);
    if (value_it == _keymap.end())
        return -1;
    return value_it->second;
}

int InputMapper::map_key(const std::string& key_name) {
    SDL_Keycode sdl_key = SDL_GetKeyFromName(key_name.c_str());
    if (sdl_key == SDLK_UNKNOWN) {
        return false;
    }
    return map_key(sdl_key);
}

} // namespace tools::sdl