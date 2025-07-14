#include "tools/misc/DynamicLibrary.hpp"

#include <stdexcept>

namespace tools {

DynamicLibrary::DynamicLibrary(const std::string &path) {
#ifdef _WIN32
    _dynlib_handle_ptr = LoadLibraryA(path.c_str());
#else
    void *handle = dlopen(path.c_str(), RTLD_LAZY);
    _dynlib_handle_ptr = std::unique_ptr<dynlib_t, DynlibDeleter>(handle);
#endif
    if (_dynlib_handle_ptr == nullptr) {
        throw std::runtime_error("Failed to load library '" + path +
                                 "'. Check that this file exist.");
    }
}

void DynamicLibrary::DynlibDeleter::operator()(dynlib_t *lib_handle) {
#ifdef _WIN32
    FreeLibrary(lib_handle);
#else
    dlclose(lib_handle);
#endif
}

} // namespace tools
