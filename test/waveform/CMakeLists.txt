set(SRC
    test_sinus.cpp
    test_square.cpp
    test_waveform_base.cpp
    test_waveform_generator.cpp
)

add_executable(test-waveform ${SRC})

target_link_libraries(test-waveform
    PRIVATE GTest::gtest_main
    PRIVATE tools-waveform
)

gtest_discover_tests(test-waveform
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES
        LABELS test-waveform
        ENVIRONMENT "TEST_OUTPUT_DIR=build/test_output/test_waveform"
)