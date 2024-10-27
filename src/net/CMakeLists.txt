set(SOURCES
    src/dns.cpp
)

set(PUBLIC_HEADERS
    include/tools/net/dns.hpp
)

target_sources(tools PRIVATE ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools PUBLIC include)
