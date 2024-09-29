#include "tools/utils/dynamic_library.hpp"

#include <stdexcept>

namespace tools::utils {

DynamicLibrary::DynamicLibrary(const std::string& path) {
#ifdef _WIN32
    _lib_instance = LoadLibraryA(path.c_str());
#else
    _lib_instance = dlopen(path.c_str(), RTLD_LAZY);
#endif
    if (_lib_instance == nullptr) {
        throw std::runtime_error("Failed to load library '" + path +
                                 "'. Check that this file exist.");
    }
}

DynamicLibrary::~DynamicLibrary() noexcept {
#ifdef _WIN32
    FreeLibrary(_lib_instance);
#else
    dlclose(_lib_instance);
#endif
    _lib_instance = nullptr;
}

} // namespace tools::utils