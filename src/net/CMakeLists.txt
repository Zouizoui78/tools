set(SOURCES
    src/dns.cpp
    src/IPAddr.cpp
    src/Pinger.cpp
)

set(PUBLIC_HEADERS
    include/tools/net/dns.hpp
    include/tools/net/IPAddr.hpp
    include/tools/net/Pinger.hpp
)

target_sources(tools PRIVATE ${SOURCES} ${PUBLIC_HEADERS})
target_include_directories(tools PUBLIC include)

if (WIN32)
    target_link_libraries(tools PRIVATE Ws2_32)
endif()