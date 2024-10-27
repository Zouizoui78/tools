#include "tools/net/dns.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <print>

namespace tools::net {

std::vector<struct addrinfo> dns_lookup(const std::string &hostname) {
    struct addrinfo *result;
    std::vector<struct addrinfo> ret;

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

    for (struct addrinfo *ai = result; ai != nullptr; ai = ai->ai_next) {
        ret.push_back(*ai);
    }

    return ret;
}

std::vector<std::string> dns_lookup_str(const std::string &hostname) {
    std::vector<std::string> ret;
    std::vector<struct addrinfo> addrinfos = dns_lookup(hostname);

    for (const struct addrinfo &ai : addrinfos) {
        void *actual_addr =
            ai.ai_family == AF_INET
                ? static_cast<void *>(
                      &(reinterpret_cast<sockaddr_in *>(ai.ai_addr)->sin_addr))
                : static_cast<void *>(&(
                      reinterpret_cast<sockaddr_in6 *>(ai.ai_addr)->sin6_addr));

        ret.emplace_back(
            ai.ai_family == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN, '\0');
        std::string &str = ret.back();

        if (inet_ntop(ai.ai_family, actual_addr, str.data(), str.size()) ==
            nullptr) {
            std::println(stderr, "inet_ntop error: {}", strerror(errno));
        }

        std::erase_if(str, [](char c) {
            return c == '\0';
        });
    }

    return ret;
}

} // namespace tools::net