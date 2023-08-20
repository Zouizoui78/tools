cmake_minimum_required(VERSION 3.17)

set(PROJECT tools)
set(VERSION 0.0.1)
set(PRJ_VER ${PROJECT}-${VERSION})

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Install vcpkg
include(FetchContent)

FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY https://github.com/microsoft/vcpkg
    GIT_TAG 2023.07.21
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg
)
FetchContent_MakeAvailable(vcpkg)

set(CMAKE_TOOLCHAIN_FILE "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")

project(
    ${PROJECT}
    VERSION ${VERSION}
    LANGUAGES CXX
)

if (WIN32)
    add_compile_options(-DWINDOWS)
endif (WIN32)
if (UNIX)
    add_compile_options(-DLINUX)

    # Needed for clangd
    add_custom_target(
        compile_commands_symlink
        ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif (UNIX)

add_compile_options($<$<CONFIG:Debug>:-DDEBUG>$<$<CONFIG:Release>:-DRELEASE>)

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

foreach(module sdl;utils;waveform)
    set(TARGET ${PROJECT}-${module})
    list(APPEND TARGETS ${TARGET})

    # Installing headers
    install(
        DIRECTORY src/${module}/include/${PROJECT}
        DESTINATION include
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
