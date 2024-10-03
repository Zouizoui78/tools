set(SRC
    test_dynamic_library.cpp
    test_endian.cpp
    test_file.cpp
    test_mutexed.cpp
    test_observable.cpp
    test_stopwatch.cpp
    test_str.cpp
    test_thread_pool.cpp
    test_time.cpp
)

target_sources(test-tools PRIVATE ${SRC})

add_library(dynlib SHARED ../resources/dynamic_library/dynlib.cpp)