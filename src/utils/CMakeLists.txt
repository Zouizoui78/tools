set(MODULE utils)
set(TARGET ${PROJECT}-${MODULE})

file(GLOB SRC_LIST src/*.cpp)

# static or shared based on configuration settings settings
add_library(
    ${TARGET}
    ${SRC_LIST}
)

target_include_directories(
    ${TARGET} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${TARGET}>  # <prefix>/include/mylib
)

target_link_libraries(${TARGET} PRIVATE spdlog::spdlog)
