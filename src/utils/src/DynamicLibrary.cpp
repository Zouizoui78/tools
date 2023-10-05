#include "tools/utils/DynamicLibrary.hpp"

namespace tools::utils {

DynamicLibrary::DynamicLibrary(const std::string &path) {
#ifdef WIN32
    _lib_instance = LoadLibraryA(path.c_str());
#else
    _lib_instance = dlopen(path.c_str(), RTLD_LAZY);
#endif
}

DynamicLibrary::~DynamicLibrary() {
    if (_lib_instance == nullptr)
        return;

#ifdef WIN32
    FreeLibrary(_lib_instance);
#else
    dlclose(_lib_instance);
#endif
    _lib_instance = nullptr;
}

bool DynamicLibrary::is_loaded() {
    return _lib_instance != nullptr;
}

} // namespace tools::utils