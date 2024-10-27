#include <arpa/inet.h>
#include <netdb.h>

namespace tools::net {

union IPAddr {
    sockaddr addr;
    sockaddr_in addr4;
    sockaddr_in6 addr6;

    IPAddr(const addrinfo *addr);
};

} // namespace tools::net