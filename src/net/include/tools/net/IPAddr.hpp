#ifndef IP_ADDR_HPP
#define IP_ADDR_HPP

#if defined(_WIN32)
#include <WS2tcpip.h>
#include <WinSock2.h>

#elif defined(__linux__)
#include <arpa/inet.h>
#include <netdb.h>

#endif

#include <string>
#include <vector>

namespace tools::net {

class IPAddr {
public:
    // Throw an invalid_argument exception if the addr is neither a valid IPV4
    // or IPV6 address.
    explicit IPAddr(std::string_view addr);

    // This ctor assumes that if addr is a valid pointer (!= nullptr), the
    // object it points to is valid.
    explicit IPAddr(const addrinfo *addr);

    std::string str() const;

    bool is_ipv4() const noexcept;
    bool is_ipv6() const noexcept;

    const struct sockaddr &addr() const noexcept;
    const struct sockaddr_in &addr4() const noexcept;
    const struct sockaddr_in6 &addr6() const noexcept;

private:
    union {
        struct sockaddr _sockaddr;
        struct sockaddr_in _sockaddr_in;
        struct sockaddr_in6 _sockaddr_in6;
    };
};

} // namespace tools::net

#endif
