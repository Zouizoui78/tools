#ifndef IP_FAMILY_HPP
#define IP_FAMILY_HPP

#if defined(_WIN32)
#include <WinSock2.h>

#elif defined(__linux__)
#include <arpa/inet.h>
#endif

namespace tools::net {

enum class IPFamily {
    UNSPEC = AF_UNSPEC,
    INET = AF_INET,
    INET6 = AF_INET6
};

} // namespace tools::net

#endif