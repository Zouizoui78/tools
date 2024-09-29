#ifndef DYNAMIC_LIBRARY_HPP
#define DYNAMIC_LIBRARY_HPP

#include <functional>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace tools::utils {

class DynamicLibrary {
private:
#ifdef _WIN32
    HINSTANCE _lib_instance = nullptr;
#else
    void* _lib_instance = nullptr;
#endif

public:
    ~DynamicLibrary() noexcept;

    DynamicLibrary(const std::string& path);
    DynamicLibrary(const DynamicLibrary& other) = delete;
    DynamicLibrary(DynamicLibrary&& other) = default;
    DynamicLibrary& operator=(const DynamicLibrary& other) = delete;
    DynamicLibrary& operator=(DynamicLibrary&& other) = default;

    template <typename R, typename... Targs>
    using Func = R (*)(Targs...);

    template <typename R, typename... Targs>
    Func<R, Targs...> get_function(const std::string& function_name) const {
#ifdef _WIN32
        return reinterpret_cast<R (*)(Targs...)>(
            GetProcAddress(_lib_instance, function_name.c_str()));
#else
        return reinterpret_cast<R (*)(Targs...)>(
            dlsym(_lib_instance, function_name.c_str()));
#endif
    }

    template <typename R, typename... Targs>
    R call(const std::string& function_name, Targs... args) const {
        Func<R, Targs...> func = get_function<R, Targs...>(function_name);
        if (func == nullptr)
            throw std::bad_function_call();
        return func(args...);
    }
};

} // namespace tools::utils

#endif // DYNAMIC_LIBRARY_HPP
