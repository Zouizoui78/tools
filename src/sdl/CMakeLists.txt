set(SOURCES
    src/InputMapper.cpp
    src/WaveformPlayer.cpp
    src/Window.cpp
)

set(PUBLIC_HEADERS
    include/tools/sdl/InputMapper.hpp
    include/tools/sdl/WaveformPlayer.hpp
    include/tools/sdl/Window.hpp
)

add_library(tools-sdl ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools-sdl PUBLIC include)
target_link_libraries(tools-sdl PUBLIC tools-utils tools-waveform)

find_package(SDL2 REQUIRED)
target_link_libraries(tools-sdl PUBLIC SDL2::SDL2)

find_package(SDL2_ttf REQUIRED)
target_link_libraries(tools-sdl PUBLIC SDL2_ttf::SDL2_ttf)
