add_library(
    dynlib
    SHARED
    ../resources/DynamicLibrary/dynlib.cpp
)

glob_tests(tools-utils "DYNLIB_PATH=${CMAKE_CURRENT_BINARY_DIR}")