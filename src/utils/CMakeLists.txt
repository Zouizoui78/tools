set(SOURCES
    src/DynamicLibrary.cpp
    src/fs.cpp
    src/Observable.cpp
    src/Scheduler.cpp
    src/Stopwatch.cpp
    src/str.cpp
    src/time.cpp
    src/Worker.cpp
)

set(PUBLIC_HEADERS
    include/tools/utils/DynamicLibrary.hpp
    include/tools/utils/fs.hpp
    include/tools/utils/Observable.hpp
    include/tools/utils/Scheduler.hpp
    include/tools/utils/Stopwatch.hpp
    include/tools/utils/str.hpp
    include/tools/utils/time.hpp
    include/tools/utils/Worker.hpp
)

add_library(tools-utils ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools-utils PUBLIC include)
