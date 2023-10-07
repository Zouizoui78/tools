cmake_minimum_required(VERSION 3.17)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(tools CXX)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(TOOLS_TESTS "Build unit tests" ON)
    if (TOOLS_TESTS)
        include(CTest)
        add_subdirectory(test)
    endif (TOOLS_TESTS)
endif()

# For clangd
if (WIN32)
    add_custom_target(compile_commands_symlink
        ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif (WIN32)
