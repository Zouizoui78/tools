set(SRC
    test_dynamic_library.cpp
    test_endian.cpp
    test_file.cpp
    test_observable.cpp
    test_str.cpp
    test_thread_pool.cpp
    test_time.cpp
)

add_executable(test-utils ${SRC})

target_link_libraries(test-utils
    PRIVATE GTest::gtest_main
    PRIVATE tools-utils
)

gtest_discover_tests(test-utils
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES
        LABELS test-utils
        ENVIRONMENT "TEST_OUTPUT_DIR=build/test_output/test_utils"
        ENVIRONMENT "DYNLIB_PATH=${CMAKE_CURRENT_BINARY_DIR}"
)

add_library(dynlib SHARED ../resources/dynamic_library/dynlib.cpp)