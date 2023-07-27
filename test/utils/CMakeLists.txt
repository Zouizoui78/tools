file(GLOB_RECURSE SRC *.cpp)

add_executable(
    test-utils
    ${SRC}
    ../main.cpp
)

add_library(
    dynlib
    SHARED
    ../resources/DynamicLibrary/dynlib.cpp
)

target_compile_features(test-utils PUBLIC cxx_std_17)

target_link_libraries(test-utils PRIVATE GTest::gtest)
target_link_libraries(test-utils PRIVATE tools-utils)

add_test(
    NAME test-utils
    COMMAND test-utils
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

set_property(
    TEST test-utils
    PROPERTY ENVIRONMENT
    "TEST_OUTPUTS=build/test_outputs/utils;DYNLIB_PATH=${CMAKE_CURRENT_BINARY_DIR}"
)