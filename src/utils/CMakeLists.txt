set(SOURCES
    src/dynamic_library.cpp
    src/file.cpp
    src/stopwatch.cpp
    src/str.cpp
    src/thread_pool.cpp
    src/time.cpp
)

set(PUBLIC_HEADERS
    include/tools/utils/dynamic_library.hpp
    include/tools/utils/file.hpp
    include/tools/utils/mutexed.hpp
    include/tools/utils/observable.hpp
    include/tools/utils/stopwatch.hpp
    include/tools/utils/str.hpp
    include/tools/utils/thread_pool.hpp
    include/tools/utils/time.hpp
)

target_sources(tools PRIVATE ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools PUBLIC include)
