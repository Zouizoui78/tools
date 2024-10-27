set(SOURCES
    src/Sinus.cpp
    src/Square.cpp
    src/WaveformBase.cpp
    src/WaveformGenerator.cpp
)

set(PUBLIC_HEADERS
    include/tools/waveform/WaveformBase.hpp
    include/tools/waveform/constants.hpp
    include/tools/waveform/Sinus.hpp
    include/tools/waveform/Square.hpp
    include/tools/waveform/WaveformGenerator.hpp
    include/tools/waveform/WaveformTimepoint.hpp
    include/tools/waveform/Waveforms.hpp
)

target_sources(tools PRIVATE ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools PUBLIC include)