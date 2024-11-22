#include "tools/net/dns.hpp"

#include <cstring>
#include <print>

#if defined(_WIN32)
#include <WinSock2.h>

#elif defined(__linux__)
#include <arpa/inet.h>

#endif

namespace tools::net::dns {

std::expected<std::vector<IPAddr>, int> lookup(std::string_view hostname,
                                               IPFamily family) {
    // Without these hints getaddrinfo returns duplicated addresses : one for
    // each socket type, doubled if an IPV6 address is found. So we filter the
    // addresses to the ones which are usable for internet sockets, which makes
    // sense since we're performing a DNS lookup.
    struct addrinfo hints {
        .ai_family = static_cast<int>(family), .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    struct addrinfo *result;
    if (int res = getaddrinfo(hostname.data(), nullptr, &hints, &result)) {
        return std::unexpected(res);
    }

    std::vector<IPAddr> ret;
    for (const struct addrinfo *ai = result; ai != nullptr; ai = ai->ai_next) {
        ret.emplace_back(ai);
    }

    freeaddrinfo(result);

    return ret;
}

} // namespace tools::net::dns