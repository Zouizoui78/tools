set(SOURCES
    src/input_mapper.cpp
    src/waveform_player.cpp
    src/window.cpp
)

set(PUBLIC_HEADERS
    include/tools/sdl/input_mapper.hpp
    include/tools/sdl/waveform_player.hpp
    include/tools/sdl/window.hpp
)

add_library(tools-sdl ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools-sdl PUBLIC include)
target_link_libraries(tools-sdl PUBLIC tools-utils tools-waveform)

find_package(SDL2 REQUIRED)
target_link_libraries(tools-sdl PUBLIC SDL2)

find_package(SDL2_ttf REQUIRED)
target_link_libraries(tools-sdl PUBLIC SDL2_ttf)
