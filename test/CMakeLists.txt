cmake_minimum_required(VERSION 3.17)

set(NAME test)

# Sources list
set(SRC_TEST
    main.cpp
    utils/TestDynamicLibrary.cpp
    utils/TestFiles.cpp
    utils/TestObservable.cpp
    utils/TestScheduler.cpp
    utils/TestStopwatch.cpp
    utils/TestTime.cpp
    utils/TestWorker.cpp
    net/TestHTTPClient.cpp
    net/TestUDP.cpp
    sdl/TestInputMapper.cpp
    sdl/TestSound.cpp
    sdl/TestWindow.cpp
)

# Project's product definition
add_executable(${NAME} EXCLUDE_FROM_ALL ${SRC_TEST})

target_link_libraries(${NAME} ${PROJECT}) # Link tested lib
target_link_libraries(${NAME} gtest)

add_custom_target(testrun
    COMMAND ${CMAKE_COMMAND} -E env TEST_OUTPUTS=${CMAKE_CURRENT_BINARY_DIR}/test_outputs ${CMAKE_COMMAND} -E env GTEST_OUTPUT="xml:${CMAKE_CURRENT_BINARY_DIR}/test_outputs/gtest_reports.xml" ${CMAKE_COMMAND} -E env GTEST_FILTER=*$(T)* ${CMAKE_CURRENT_BINARY_DIR}/${NAME}
    DEPENDS ${NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running tests..."
)

# Symlink to access test resources
add_custom_command(
    TARGET ${NAME}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/test_resources
)

# Compile small shared library used in tests
set(DYN_LIB_TEST shared)
add_library(
    ${DYN_LIB_TEST}
    SHARED
    EXCLUDE_FROM_ALL
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/DynamicLibrary/test.cpp
)

add_dependencies(${NAME} ${DYN_LIB_TEST})