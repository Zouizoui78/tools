find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_executable(test-tools)
add_subdirectory(utils)
add_subdirectory(waveform)

target_link_libraries(test-tools
    PRIVATE GTest::gtest_main
    PRIVATE tools
)

gtest_discover_tests(test-tools
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES
        LABELS test-tools
        ENVIRONMENT "TEST_OUTPUT_DIR=build/test_output"
        ENVIRONMENT "DYNLIB_PATH=${CMAKE_CURRENT_BINARY_DIR}/utils"
)
