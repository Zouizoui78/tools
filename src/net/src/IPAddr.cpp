#include "tools/net/IPAddr.hpp"
#include "tools/net/dns.hpp"

#include <cstring>
#include <format>
#include <print>
#include <stdexcept>

namespace tools::net {

IPAddr::IPAddr(std::string_view addr) {
    if (inet_pton(AF_INET, addr.data(), &_sockaddr_in.sin_addr)) {
        _sockaddr.sa_family = AF_INET;
        return;
    }

    if (inet_pton(AF_INET6, addr.data(), &_sockaddr_in6.sin6_addr)) {
        _sockaddr.sa_family = AF_INET6;
        return;
    }

    throw std::invalid_argument(
        std::format("'{}' is not a valid IPV4 or IPV6 address", addr));
}

IPAddr::IPAddr(const addrinfo *addr) {
    if (addr == nullptr) {
        throw std::invalid_argument("Null addr pointer");
    }

    if (addr->ai_family != AF_INET && addr->ai_family != AF_INET6) {
        throw std::runtime_error("Unsupported address type");
    }

    memcpy(&_sockaddr, addr->ai_addr, addr->ai_addrlen);
}

std::string IPAddr::str() const noexcept {
    std::string ret;
    const void *addr = nullptr;

    if (this->is_ipv4()) {
        ret.resize(INET_ADDRSTRLEN, '\0');
        addr = &(_sockaddr_in.sin_addr);
    }
    else if (this->is_ipv6()) {
        ret.resize(INET6_ADDRSTRLEN, '\0');
        addr = &(_sockaddr_in6.sin6_addr);
    }
    else {
        return "";
    }

    if (inet_ntop(_sockaddr.sa_family, addr, ret.data(), ret.size()) ==
        nullptr) {
        std::println("{}", strerror(errno));
    }

    std::erase_if(ret, [](char c) {
        return c == '\0';
    });

    return ret;
}

bool IPAddr::is_ipv4() const noexcept {
    return _sockaddr.sa_family == AF_INET;
}

bool IPAddr::is_ipv6() const noexcept {
    return _sockaddr.sa_family == AF_INET6;
}

const struct sockaddr_in &IPAddr::addr4() const noexcept {
    return _sockaddr_in;
}

const struct sockaddr_in6 &IPAddr::addr6() const noexcept {
    return _sockaddr_in6;
}

} // namespace tools::net