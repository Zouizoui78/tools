set(SRC
    TestInputMapper.cpp
    TestWindow.cpp
)

option(TEST_WAVEFORM_PLAYER "Enable waveform player tests" OFF)
if (TEST_WAVEFORM_PLAYER)
    set(SRC
        ${SRC}
        TestWaveformPlayer.cpp
    )
endif (TEST_WAVEFORM_PLAYER)

add_executable(test-sdl ${SRC})

target_link_libraries(test-sdl
    PRIVATE GTest::gtest_main
    PRIVATE tools-sdl
    PRIVATE tools-utils
    PRIVATE tools-waveform
)

gtest_discover_tests(test-sdl
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES
        ENVIRONMENT "TEST_OUTPUT_DIR=build/TestOutput/TestSdl"
)