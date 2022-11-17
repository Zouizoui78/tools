cmake_minimum_required(VERSION 3.17)

set(PROJECT tools)
set(VERSION 0.1)
set(PRJ_VER ${PROJECT}-${VERSION})

# C++ variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# OS defines used inside code
if (WIN32)
    add_compile_options(-DWINDOWS)
endif (WIN32)
if (UNIX)
    add_compile_options(-DLINUX)
endif (UNIX)

add_compile_options($<$<CONFIG:Debug>:-DDEBUG>$<$<CONFIG:Release>:-DRELEASE>)

# set the project name
project(
    ${PROJECT}
    VERSION ${VERSION}
    LANGUAGES CXX
)

include(src/sources_list.cmake)

# Project's product definition
add_library(${PROJECT} SHARED ${SRC})

target_include_directories(
    ${PROJECT}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT} curl)
target_link_libraries(${PROJECT} fmt)
target_link_libraries(${PROJECT} SDL2)
target_link_libraries(${PROJECT} SDL2_ttf)
target_link_libraries(${PROJECT} spdlog)

add_subdirectory(test)

set(INSTALL_INCLUDEDIR include)
set(INSTALL_LIBDIR lib)

# Installing headers
install(
    DIRECTORY include/
    DESTINATION ${INSTALL_INCLUDEDIR}
)

# Installing binary
install(
    TARGETS ${PROJECT}
    DESTINATION ${INSTALL_LIBDIR}
    EXPORT ${PROJECT}ConfigTargets
)