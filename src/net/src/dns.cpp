#include "tools/net/dns.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <print>

namespace tools::net {

std::vector<IPAddr> dns_lookup(const std::string &hostname) {
    struct addrinfo *result;
    std::vector<IPAddr> ret;

    // Without this getaddrinfo returns duplicated addresses : one for each
    // socket type, doubled if an IPV6 address is found.
    // So we filter the addresses to the ones which are usable for internet
    // sockets, which makes sense since we're performing a DNS lookup.
    struct addrinfo hints {
        .ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM,
        .ai_protocol = IPPROTO_TCP
    };

    int res = getaddrinfo(hostname.c_str(), nullptr, &hints, &result);
    if (res != 0) {
        std::println(stderr, "getaddrinfo error: {}", gai_strerror(res));
        return ret;
    }

    for (const struct addrinfo *ai = result; ai != nullptr; ai = ai->ai_next) {
        ret.emplace_back(ai);
    }

    freeaddrinfo(result);

    return ret;
}

std::vector<std::string> dns_lookup_str(const std::string &hostname) {
    std::vector<std::string> ret;

    for (const IPAddr &addr : dns_lookup(hostname)) {
        const void *concrete_addr = nullptr;
        int addr_len = 0;

        switch (addr.addr.sa_family) {
        case AF_INET:
            concrete_addr = &(addr.addr4.sin_addr);
            addr_len = INET_ADDRSTRLEN;
            break;
        case AF_INET6:
            concrete_addr = &(addr.addr6.sin6_addr);
            addr_len = INET6_ADDRSTRLEN;
            break;
        }

        ret.emplace_back(addr_len, '\0');
        std::string &str = ret.back();

        if (inet_ntop(addr.addr.sa_family, concrete_addr, str.data(),
                      str.size()) == nullptr) {
            std::println(stderr, "inet_ntop error: {}", strerror(errno));
        }

        std::erase_if(str, [](char c) {
            return c == '\0';
        });
    }

    return ret;
}

} // namespace tools::net