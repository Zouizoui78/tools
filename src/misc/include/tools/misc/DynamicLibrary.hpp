#ifndef DYNAMIC_LIBRARY_HPP
#define DYNAMIC_LIBRARY_HPP

#include <functional>
#include <memory>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace tools {

class DynamicLibrary {
private:
#ifdef _WIN32
    using dynlib_t = HINSTANCE;
#else
    using dynlib_t = void;
#endif
    struct DynlibDeleter {
        void operator()(dynlib_t *lib_handle);
    };
    std::unique_ptr<dynlib_t, DynlibDeleter> _dynlib_handle_ptr;

public:
    DynamicLibrary(const std::string &path);

    template <typename R, typename... Targs>
    using FuncPtr = R (*)(Targs...);

    template <typename R, typename... Targs>
    FuncPtr<R, Targs...>
    get_function_addr(const std::string &function_name) const {
#ifdef _WIN32
        return reinterpret_cast<R (*)(Targs...)>(
            GetProcAddress(_dynlib_handle_ptr.get(), function_name.c_str()));
#else
        return reinterpret_cast<R (*)(Targs...)>(
            dlsym(_dynlib_handle_ptr.get(), function_name.c_str()));
#endif
    }

    template <typename R, typename... Targs>
    std::function<R(Targs...)>
    get_function(const std::string &function_name) const {
        return get_function_addr<R, Targs...>(function_name);
    }

    template <typename R, typename... Targs>
    R call(const std::string &function_name, Targs... args) const {
        auto func = get_function_addr<R, Targs...>(function_name);
        if (func == nullptr) {
            throw std::bad_function_call();
        }
        return func(args...);
    }
};

} // namespace tools

#endif // DYNAMIC_LIBRARY_HPP
