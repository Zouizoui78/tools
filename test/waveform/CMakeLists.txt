set(SRC
    test_sinus.cpp
    test_square.cpp
    test_waveform_base.cpp
    test_waveform_generator.cpp
)

target_sources(test-tools PRIVATE ${SRC})