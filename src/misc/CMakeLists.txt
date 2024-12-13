set(SOURCES
    src/DynamicLibrary.cpp
    src/file.cpp
    src/Stopwatch.cpp
    src/str.cpp
    src/ThreadPool.cpp
    src/time.cpp
    src/UUID.cpp
)

set(PUBLIC_HEADERS
    include/tools/misc/DynamicLibrary.hpp
    include/tools/misc/file.hpp
    include/tools/misc/Observable.hpp
    include/tools/misc/Stopwatch.hpp
    include/tools/misc/str.hpp
    include/tools/misc/ThreadPool.hpp
    include/tools/misc/time.hpp
    include/tools/misc/UUID.hpp
)

target_sources(tools PRIVATE ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools PUBLIC include)
