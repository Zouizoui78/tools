set(SRC
    TestDynamicLibrary.cpp
    TestEndian.cpp
    TestFile.cpp
    TestObservable.cpp
    TestStopwatch.cpp
    TestStr.cpp
    TestThreadPool.cpp
    TestTime.cpp
    TestUUID.cpp
)

target_sources(test-tools PRIVATE ${SRC})

add_library(dynlib SHARED ../resources/DynamicLibrary/dynlib.cpp)

if (WIN32)
    target_link_libraries(test-tools PRIVATE Rpcrt4)
else()
    target_link_libraries(test-tools PRIVATE uuid)
endif ()

set(DYNLIB_OUTPUT_PATH ${CMAKE_BINARY_DIR}/test-dynlib)

# Set path that will be set in cmake/test_paths.hpp.in
# by the configure_file command in test/CMakeLists.txt.
set(DYNLIB_OUTPUT_PATH ${DYNLIB_OUTPUT_PATH} PARENT_SCOPE)

set_target_properties(dynlib
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${DYNLIB_OUTPUT_PATH}
    LIBRARY_OUTPUT_DIRECTORY ${DYNLIB_OUTPUT_PATH}
    RUNTIME_OUTPUT_DIRECTORY ${DYNLIB_OUTPUT_PATH}
)
