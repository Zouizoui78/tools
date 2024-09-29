#include <string>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" EXPORT bool func(const std::string& str) {
    return str == "test";
}

extern "C" EXPORT int add(int a, int b) {
    return a + b;
}