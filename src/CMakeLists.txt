add_subdirectory(utils)

option(TOOLS_SDL "Build tools-sdl module" ON)
option(TOOLS_WAVEFORM "Build tools-waveform module" ON)

if (TOOLS_SDL)
    add_subdirectory(sdl)
endif (TOOLS_SDL)

if (TOOLS_WAVEFORM)
    add_subdirectory(waveform)
endif (TOOLS_WAVEFORM)
