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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ${PROJECT}
    curl
    fmt
    SDL2
    SDL2_ttf
    spdlog
)

add_subdirectory(test)

set(INSTALL_INCLUDEDIR include)
set(INSTALL_LIBDIR lib)
set(INSTALL_CMAKEDIR lib/cmake/${PROJECT})

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

# Cmake build files
install(
    EXPORT ${PROJECT}ConfigTargets
    DESTINATION ${INSTALL_CMAKEDIR}
)

# Generate config files
include(CMakePackageConfigHelpers)
configure_package_config_file( 
    cmake/${PROJECT}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}Config.cmake
    DESTINATION ${INSTALL_CMAKEDIR}
)