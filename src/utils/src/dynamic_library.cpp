#include "tools/utils/dynamic_library.hpp"

#include <format>
#include <stdexcept>

namespace tools::utils {

DynamicLibrary::DynamicLibrary(const std::string& path) {
#ifdef WIN32
    _lib_instance = LoadLibraryA(path.c_str());
#else
    _lib_instance = dlopen(path.c_str(), RTLD_LAZY);
#endif
    if (_lib_instance == nullptr) {
        throw std::runtime_error(std::format(
            "Failed to load library '{}'. Check that this file exist.", path));
    }
}

DynamicLibrary::~DynamicLibrary() {
#ifdef WIN32
    FreeLibrary(_lib_instance);
#else
    dlclose(_lib_instance);
#endif
    _lib_instance = nullptr;
}

} // namespace tools::utils