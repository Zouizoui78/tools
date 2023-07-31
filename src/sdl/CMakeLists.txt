set(MODULE sdl)
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

target_link_libraries(
    ${TARGET}
    PRIVATE
    nlohmann_json::nlohmann_json
)

target_link_libraries(
    ${TARGET}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_link_libraries(
    ${TARGET}
    PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

target_link_libraries(${TARGET} PRIVATE spdlog::spdlog)
target_link_libraries(${TARGET} PRIVATE tools-utils)


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