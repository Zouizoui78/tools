include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(test-tools)
add_subdirectory(misc)
add_subdirectory(net)
add_subdirectory(waveform)

target_link_libraries(test-tools
    PRIVATE GTest::gtest_main
    PRIVATE tools
)

if (UNIX)
    target_compile_options(tools PRIVATE --coverage)
    target_link_options(test-tools PRIVATE --coverage)
endif (UNIX)

gtest_discover_tests(test-tools
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES
        ENVIRONMENT "TEST_OUTPUT_DIR=build/test_output"
        ENVIRONMENT "DYNLIB_PATH=$<TARGET_FILE:dynlib>"
)
