cmake_minimum_required(VERSION 3.17)

set (CMAKE_CXX_STANDARD 23)
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