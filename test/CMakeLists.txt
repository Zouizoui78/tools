include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
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
