file(GLOB_RECURSE SRC *.cpp)

add_executable(
    test-sdl
    ${SRC}
    ../main.cpp
)

target_compile_features(test-sdl PUBLIC cxx_std_17)

target_link_libraries(test-sdl PRIVATE GTest::gtest)
target_link_libraries(test-sdl PRIVATE tools-sdl)
target_link_libraries(test-sdl PRIVATE tools-utils)

add_test(
    NAME test-sdl
    COMMAND test-sdl
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

set_property(
    TEST test-sdl
    PROPERTY ENVIRONMENT
    "TEST_OUTPUTS=build/test_outputs/sdl"
)