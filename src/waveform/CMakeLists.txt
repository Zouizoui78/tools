set(SOURCES
    src/awaveform.cpp
    src/sinus.cpp
    src/square.cpp
    src/waveform_generator.cpp
)

set(PUBLIC_HEADERS
    include/tools/waveform/awaveform.hpp
    include/tools/waveform/constants.hpp
    include/tools/waveform/sinus.hpp
    include/tools/waveform/square.hpp
    include/tools/waveform/waveform_generator.hpp
    include/tools/waveform/waveform_timepoint.hpp
    include/tools/waveform/waveforms.hpp
)

add_library(tools-waveform ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools-waveform PUBLIC include)
