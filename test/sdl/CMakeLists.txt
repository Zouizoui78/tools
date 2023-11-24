set(SRC
    test_input_mapper.cpp
    test_sdl_audio_device.cpp
)

if (TOOLS_WAVEFORM_PLAYER_TESTS)
    set(SRC
        ${SRC}
        test_waveform_player.cpp
    )
endif (TOOLS_WAVEFORM_PLAYER_TESTS)

if (TOOLS_WINDOW_TESTS)
    set(SRC
        ${SRC}
        test_window.cpp
    )
endif (TOOLS_WINDOW_TESTS)

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
        LABELS test-sdl
        ENVIRONMENT "TEST_OUTPUT_DIR=build/test_output/test_sdl"
)