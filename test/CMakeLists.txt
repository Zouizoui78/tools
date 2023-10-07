include(../cmake/GlobTests.cmake)

find_package(GTest CONFIG REQUIRED)

add_subdirectory(sdl)
add_subdirectory(utils)
add_subdirectory(waveform)
