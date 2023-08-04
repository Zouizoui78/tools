set(MODULE waveform)
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
