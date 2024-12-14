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