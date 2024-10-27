set(SOURCES
    src/dns.cpp
    src/ip_addr.cpp
)

set(PUBLIC_HEADERS
    include/tools/net/dns.hpp
    include/tools/net/ip_addr.hpp
)

target_sources(tools PRIVATE ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools PUBLIC include)
