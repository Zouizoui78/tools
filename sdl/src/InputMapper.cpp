#include "tools/sdl/InputMapper.hpp"
#include "tools/utils/Log.hpp"

namespace tools::sdl {

static auto logger = tools::utils::new_logger("InputMapper");

InputMapper::InputMapper() {}
    
InputMapper::~InputMapper() {}

bool InputMapper::add_mapping(SDL_Keycode key, uint8_t mapped_key) {
    if (_keymap.contains(key)) {
        logger->error("Keymap already contains '{}'.", key);
        return false;
    }
    _keymap[key] = mapped_key;
    return true;
}

bool InputMapper::add_mapping(const std::string &key, uint8_t mapped_key) {
    SDL_Keycode sdl_key = SDL_GetKeyFromName(key.c_str());
    if (sdl_key == SDLK_UNKNOWN) {
        logger->error("Unknown key '{}'", key);
        return false;
    }
    return add_mapping(sdl_key, mapped_key);
}

bool InputMapper::remove_mapping(SDL_Keycode key) {
    if (!_keymap.contains(key)) {
        logger->error("Keymap doesn't contain '{}'.", key);
        return false;
    }
    _keymap.erase(key);
    return true;
}

bool InputMapper::remove_mapping(const std::string &key) {
    SDL_Keycode sdl_key = SDL_GetKeyFromName(key.c_str());
    if (sdl_key == SDLK_UNKNOWN) {
        logger->error("Unknown key '{}'", key);
        return false;
    }
    return remove_mapping(sdl_key);
}

bool InputMapper::set_map(const json &map) {
    bool success = true;
    for (const auto &e : map.items()) {
        SDL_Keycode key = SDL_GetKeyFromName(e.key().c_str());
        if (key == SDLK_UNKNOWN) {
            logger->error("Unknown key : '{}' ; error : {}", e.key(), SDL_GetError());
            success = false;
        }
        if (!add_mapping(key, e.value()))
            success = false;
    }
    return success;
}

int InputMapper::map_key(SDL_Keycode key) {
    if (!_keymap.contains(key))
        return -1;
    return _keymap[key];
}

int InputMapper::map_key(const std::string &key) {
    SDL_Keycode sdl_key = SDL_GetKeyFromName(key.c_str());
    if (sdl_key == SDLK_UNKNOWN) {
        logger->error("Unknown key '{}'", key);
        return false;
    }
    return map_key(sdl_key);
}

} // namespace tools::sdl