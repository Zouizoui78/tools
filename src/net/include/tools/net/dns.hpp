#ifndef DNS_HPP
#define DNS_HPP

#include <expected>
#include <string>
#include <vector>

#include "AddressFamily.hpp"
#include "IPAddr.hpp"
#include "NetIncludes.hpp"

namespace tools::net::dns {

// Wrapper calling getaddrinfo. The addresses are stored in
// the returned vector. For error codes and explanations, see `man getaddrinfo`.
std::expected<std::vector<IPAddr>, int>
lookup(std::string_view hostname, AddressFamily family = AddressFamily::UNSPEC);

// Wrapper calling getnameinfo. For error codes and explanations, see `man
// getnameinfo`.
std::expected<std::string, int> rlookup(const IPAddr &addr);

} // namespace tools::net::dns

#endif
