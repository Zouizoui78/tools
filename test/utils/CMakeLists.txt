set(SRC
    TestDynamicLibrary.cpp
    TestEndian.cpp
    TestFile.cpp
    TestObservable.cpp
    TestScheduler.cpp
    TestStopwatch.cpp
    TestStr.cpp
    TestTime.cpp
    TestWorker.cpp
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
        ENVIRONMENT "TEST_OUTPUT_DIR=build/TestOutput/TestUtils"
        ENVIRONMENT "DYNLIB_PATH=${CMAKE_CURRENT_BINARY_DIR}"
)

add_library(dynlib SHARED ../resources/DynamicLibrary/dynlib.cpp)