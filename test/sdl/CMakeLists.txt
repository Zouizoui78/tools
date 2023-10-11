set(SRC
    test_input_mapper.cpp
    test_window.cpp
)

option(TEST_WAVEFORM_PLAYER "Enable waveform player tests" OFF)
if (TEST_WAVEFORM_PLAYER)
    set(SRC
        ${SRC}
        test_waveform_player.cpp
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
        LABELS test-sdl
        ENVIRONMENT "TEST_OUTPUT_DIR=build/test_output/test_sdl"
)