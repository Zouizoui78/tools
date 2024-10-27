set(SRC
    TestDynamicLibrary.cpp
    TestEndian.cpp
    TestFile.cpp
    TestObservable.cpp
    TestStopwatch.cpp
    TestStr.cpp
    TestThreadPool.cpp
    TestTime.cpp
)

target_sources(test-tools PRIVATE ${SRC})

add_library(dynlib SHARED ../resources/DynamicLibrary/dynlib.cpp)