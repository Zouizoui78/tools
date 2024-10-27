set(SOURCES
    src/DynamicLibrary.cpp
    src/file.cpp
    src/Stopwatch.cpp
    src/str.cpp
    src/ThreadPool.cpp
    src/time.cpp
)

set(PUBLIC_HEADERS
    include/tools/utils/DynamicLibrary.hpp
    include/tools/utils/file.hpp
    include/tools/utils/Observable.hpp
    include/tools/utils/Stopwatch.hpp
    include/tools/utils/str.hpp
    include/tools/utils/ThreadPool.hpp
    include/tools/utils/time.hpp
)

target_sources(tools PRIVATE ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools PUBLIC include)
