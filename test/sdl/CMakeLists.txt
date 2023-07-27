file(GLOB_RECURSE SRC *.cpp)

add_executable(
    test-sdl
    ${SRC}
)

target_compile_features(test-sdl PUBLIC cxx_std_17)

target_link_libraries(test-sdl PRIVATE spdlog::spdlog)

target_link_libraries(test-sdl PRIVATE tools-sdl)
target_link_libraries(test-sdl PRIVATE tools-utils)

target_link_libraries(test-sdl PRIVATE GTest::gtest)

add_test(NAME test-sdl-test COMMAND test-sdl)
set_property(TEST test-sdl-test PROPERTY ENVIRONMENT "TEST_OUTPUTS=build/test_outputs")