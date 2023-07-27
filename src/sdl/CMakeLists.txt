set(MODULE sdl)

file(GLOB SRC_LIST src/*.cpp)

# static or shared based on configuration settings settings
add_library(
    ${PROJECT}-${MODULE}
    ${SRC_LIST}
)

target_compile_features(${PROJECT}-${MODULE} PUBLIC cxx_std_20)

target_include_directories(${PROJECT}-${MODULE} PUBLIC include)

target_link_libraries(
    ${PROJECT}-${MODULE}
    PRIVATE
    nlohmann_json::nlohmann_json
)

target_link_libraries(
    ${PROJECT}-${MODULE}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_link_libraries(
    ${PROJECT}-${MODULE}
    PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

target_link_libraries(${PROJECT}-${MODULE} PRIVATE spdlog::spdlog)
target_link_libraries(${PROJECT}-${MODULE} PRIVATE tools-utils)