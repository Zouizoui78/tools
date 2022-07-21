#include "tools/sdl/InputMapper.hpp"
#include "tools/utils/Log.hpp"

namespace tools::sdl {

static auto logger = tools::utils::new_logger("InputMapper");

InputMapper::InputMapper() {}
    
InputMapper::~InputMapper() {}

bool InputMapper::add_mapping(const std::string &key, uint8_t mapped_key) {
    if (_keymap.contains(key)) {
        logger->error("Keymap already contains '{}'.", key);
        return false;
    }
    _keymap[key] = mapped_key;
    return true;
}

bool InputMapper::remove_mapping(const std::string &key) {
    if (!_keymap.contains(key)) {
        logger->error("Keymap doesn't contain '{}'.", key);
        return false;
    }
    _keymap.erase(key);
    return true;
}

bool InputMapper::set_map(const json &map) {
    bool success = true;
    for (const auto &e : map.items()) {
        if (!add_mapping(e.key(), e.value()))
            success = false;
    }
    return success;
}

int InputMapper::map_key(const std::string &key) {
    if (!_keymap.contains(key))
        return -1;
    return _keymap[key];
}

} // namespace tools::sdl