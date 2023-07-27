set(MODULE utils)

file(GLOB SRC_LIST src/*.cpp)

# static or shared based on configuration settings settings
add_library(
    ${PROJECT}-${MODULE}
    ${SRC_LIST}
)

target_compile_features(${PROJECT}-${MODULE} PUBLIC cxx_std_17)

target_include_directories(${PROJECT}-${MODULE} PUBLIC include)

target_link_libraries(${PROJECT}-${MODULE} PRIVATE spdlog::spdlog)