cmake_minimum_required(VERSION 3.17)

set(PROJECT tools)
set(VERSION 0.1)
set(PRJ_VER ${PROJECT}-${VERSION})

project(
    ${PROJECT}
    VERSION ${VERSION}
    LANGUAGES CXX
)

# Options shared across modules
if (WIN32)
    add_compile_options(-DWINDOWS)
endif (WIN32)
if (UNIX)
    add_compile_options(-DLINUX)
endif (UNIX)

add_compile_options($<$<CONFIG:Debug>:-DDEBUG>$<$<CONFIG:Release>:-DRELEASE>)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
    include(CTest)
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(test)
endif()


# Install

set(INSTALL_LIBDIR lib)
set(INSTALL_CMAKEDIR lib/cmake/${PROJECT})

foreach(module sdl;utils)
    set(TARGET ${PROJECT}-${module})
    list(APPEND TARGETS ${TARGET})

    # Installing headers
    install(
        DIRECTORY src/${module}/include/${module}
        DESTINATION include/${PROJECT}
    )
endforeach()

# Installing binary
install(
    TARGETS ${TARGETS}
    DESTINATION ${INSTALL_LIBDIR}
    EXPORT ${PROJECT}Targets
)

# Install cmake config file
install(
    EXPORT ${PROJECT}Targets
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
