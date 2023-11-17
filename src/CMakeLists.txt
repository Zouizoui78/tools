add_subdirectory(utils)

if (TOOLS_SDL)
    add_subdirectory(sdl)
endif (TOOLS_SDL)

if (TOOLS_WAVEFORM)
    add_subdirectory(waveform)
endif (TOOLS_WAVEFORM)
