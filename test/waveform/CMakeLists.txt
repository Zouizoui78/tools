set(SRC
    TestAWaveform.cpp
    TestSinus.cpp
    TestSquare.cpp
    TestWaveformGenerator.cpp
)

add_executable(test-waveform ${SRC})

target_link_libraries(test-waveform
    PRIVATE GTest::gtest_main
    PRIVATE tools-waveform
)

gtest_discover_tests(test-waveform
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES
        ENVIRONMENT "TEST_OUTPUT_DIR=build/TestOutput/TestWaveform"
)