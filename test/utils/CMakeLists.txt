file(GLOB_RECURSE SRC *.cpp)

add_executable(
    test-utils
    ${SRC}
    ../main.cpp
)

target_compile_features(test-utils PUBLIC cxx_std_17)

target_link_libraries(test-utils PRIVATE spdlog::spdlog)

target_link_libraries(test-utils PRIVATE tools-utils)

target_link_libraries(test-utils PRIVATE GTest::gtest)

add_test(
    NAME
    test-utils
    COMMAND
    test-utils
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
set_property(TEST test-utils PROPERTY ENVIRONMENT "TEST_OUTPUTS=build/test_outputs")