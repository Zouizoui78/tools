cmake_minimum_required(VERSION 3.13)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(tools CXX)

add_library(tools)
add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(TOOLS_TESTS "Build unit tests" OFF)
    if (TOOLS_TESTS)
        include(CTest)
        add_subdirectory(test)

        if (UNIX)
            target_compile_options(tools PRIVATE --coverage)
            target_link_libraries(tools PRIVATE gcov)
        endif (UNIX)
    endif (TOOLS_TESTS)
endif()