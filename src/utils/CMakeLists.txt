set(MODULE utils)
set(TARGET ${PROJECT}-${MODULE})

file(GLOB SRC_LIST src/*.cpp)

# static or shared based on configuration settings settings
add_library(
    ${TARGET}
    ${SRC_LIST}
)

target_include_directories(
    ${TARGET} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${TARGET}>  # <prefix>/include/mylib
)

target_link_libraries(${TARGET} PRIVATE spdlog::spdlog)


# Install

set(INSTALL_INCLUDEDIR include)
set(INSTALL_LIBDIR lib)
set(INSTALL_CMAKEDIR lib/cmake/${PROJECT})

# Installing headers
install(
    DIRECTORY include
    DESTINATION ${INSTALL_INCLUDEDIR}
)

# Installing binary
install(
    TARGETS ${TARGET}
    DESTINATION ${INSTALL_LIBDIR}
    EXPORT ${TARGET}Targets
)

# Cmake build files
install(
    EXPORT ${TARGET}Targets
    DESTINATION ${INSTALL_CMAKEDIR}
)

# # Generate config files
# include(CMakePackageConfigHelpers)
# configure_package_config_file(
#     cmake/${PROJECT}Config.cmake.in
#     ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}Config.cmake
#     INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
# )

# install(
#     FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT}Config.cmake
#     DESTINATION ${INSTALL_CMAKEDIR}
# )