function(glob_tests LINKED_LIBS)
    file(GLOB SRC *.cpp)

    foreach(file ${SRC})
        get_filename_component(target ${file} NAME_WE)

        add_executable(
            ${target}
            ${file}
            ../main.cpp
        )
        
        find_package(spdlog REQUIRED)

        target_link_libraries(
            ${target}
            PRIVATE GTest::gtest
            PRIVATE spdlog::spdlog
        )

        foreach(LIB ${LINKED_LIBS})
            target_link_libraries(${target} PRIVATE ${LIB})
        endforeach(LIB ${LINKED_LIBS})

        add_test(
            NAME ${target}
            COMMAND ${target}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )

        set_property(
            TEST ${target}
            PROPERTY ENVIRONMENT
            "TEST_OUTPUTS=build/test_outputs/${target};${ARGN}"
        )

        set_property(
            TEST ${target}
            PROPERTY LABELS ${target}
        )
    endforeach()
endfunction(glob_tests)
