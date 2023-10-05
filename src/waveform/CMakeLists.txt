set(SOURCES
    src/AWaveform.cpp
    src/Sinus.cpp
    src/Square.cpp
    src/WaveformGenerator.cpp
)

set(PUBLIC_HEADERS
    include/tools/waveform/AWaveform.hpp
    include/tools/waveform/Constants.hpp
    include/tools/waveform/Sinus.hpp
    include/tools/waveform/Square.hpp
    include/tools/waveform/WaveformGenerator.hpp
    include/tools/waveform/WaveformTimepoint.hpp
    include/tools/waveform/Waveforms.hpp
)

add_library(tools-waveform ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools-waveform PUBLIC include)
