#ifndef DYNAMIC_LIBRARY_HPP
#define DYNAMIC_LIBRARY_HPP

#include <functional>
#include <string>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace tools::utils {

/**
 * @brief Wrapper to load dynamic libraries and call code.
 */
class DynamicLibrary {
    public:

    DynamicLibrary(const std::string &path);
    ~DynamicLibrary();

    /**
     * @brief Tell whether the dynamic library was succesfully loaded or not.
     * @return yes or no.
     */
    bool is_loaded();

    /**
     * @brief Load a function from the dynamic library.
     * @tparam R Return type of the wanted function.
     * @tparam Targs Arguments types of the loaded function.
     * @param function_name Name of the function to load.
     * @return Function pointer if found, nullptr if not found.
     */
    template <typename R, typename ... Targs>
    R (*get_function(const std::string &function_name))(Targs ...) {
    #ifdef WIN32
        std::cout << "salut\n";
        // Avoid a stupid warning on windows.
        void *tmp = reinterpret_cast<void *>(GetProcAddress(_lib_instance, function_name.c_str()));
        return reinterpret_cast<R (*)(Targs ...)>(tmp);
    #else
        return reinterpret_cast<R (*)(Targs ...)>(dlsym(_lib_instance, function_name.c_str()));
    #endif
    }

    /**
     * @brief Call a function from the dynamic library. If the function doesn't exist, a std::bad_function_call exception is thrown.
     * @tparam R Return type of the function.
     * @tparam Targs Arguments types.
     * @param function_name Name of the function.
     * @param args Function arguments.
     * @return Value returned from the function.
     */
    template <typename R, typename ...Targs>
    R call(const std::string &function_name, Targs ... args) {
        R (*func_ptr)(Targs ...) = get_function<R, Targs ...>(function_name);
        if (func_ptr == nullptr)
            throw std::bad_function_call();
        return func_ptr(args ...);
    }

    private:
#ifdef WIN32
    HINSTANCE _lib_instance = nullptr;
#else
    void *_lib_instance = nullptr;
#endif
};

} // namespace tools::utils

#endif // DYNAMIC_LIBRARY_HPP
