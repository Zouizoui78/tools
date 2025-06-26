#ifndef DNS_HPP
#define DNS_HPP

#if defined(_WIN32)
#include <WinSock2.h>

#elif defined(__linux__)
#include <arpa/inet.h>

#endif

#include <expected>
#include <string>
#include <vector>

#include "IPAddr.hpp"
#include "IPFamily.hpp"

namespace tools::net::dns {

// Wrapper calling getaddrinfo. The addresses are stored in
// the returned vector. For error codes and explanations, see `man getaddrinfo`.
std::expected<std::vector<IPAddr>, int>
lookup(std::string_view hostname, IPFamily family = IPFamily::UNSPEC);

// Wrapper calling getnameinfo. For error codes and explanations, see `man
// getnameinfo`.
std::expected<std::string, int> rlookup(const IPAddr &addr);

} // namespace tools::net::dns

#endif
