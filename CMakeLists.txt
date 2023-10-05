cmake_minimum_required(VERSION 3.17)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(tools CXX)

option(TOOLS_BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
    AND BUILD_TESTS)
    include(CTest)
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(test)
endif()

# For clangd
add_custom_target(compile_commands_symlink
    ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
)
