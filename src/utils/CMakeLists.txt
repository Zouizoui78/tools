set(SOURCES
    src/dynamic_library.cpp
    src/file.cpp
    src/stopwatch.cpp
    src/str.cpp
    src/time.cpp
)

set(PUBLIC_HEADERS
    include/tools/utils/dynamic_library.hpp
    include/tools/utils/file.hpp
    include/tools/utils/observable.hpp
    include/tools/utils/stopwatch.hpp
    include/tools/utils/str.hpp
    include/tools/utils/time.hpp
)

add_library(tools-utils ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools-utils PUBLIC include)
