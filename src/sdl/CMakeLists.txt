set(MODULE sdl)
set(TARGET ${PROJECT}-${MODULE})

file(GLOB SRC_LIST src/*.cpp)

add_library(
    ${TARGET}
    ${SRC_LIST}
)

target_include_directories(
    ${TARGET} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ${TARGET}
    PRIVATE
    nlohmann_json::nlohmann_json
)

target_link_libraries(
    ${TARGET}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_link_libraries(
    ${TARGET}
    PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

target_link_libraries(${TARGET} PRIVATE spdlog::spdlog)
target_link_libraries(${TARGET} PRIVATE tools-utils tools-waveform)
