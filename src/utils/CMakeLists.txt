set(MODULE utils)

file(GLOB SRC_LIST src/*.cpp)

# static or shared based on configuration settings settings
add_library(
    ${PROJECT}-${MODULE}
    ${SRC_LIST}
)

target_include_directories(${PROJECT}-${MODULE} PUBLIC include)

target_link_libraries(${PROJECT}-${MODULE} PRIVATE spdlog::spdlog)