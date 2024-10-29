#ifndef DNS_HPP
#define DNS_HPP

#include <string>
#include <vector>

#include "IPAddr.hpp"

namespace tools::net {

// Return an empty vector if the hostname doesn't exist.
std::vector<IPAddr> dns_lookup(const std::string &hostname);

// Return an empty vector if the hostname doesn't exist.
std::vector<std::string> dns_lookup_str(const std::string &hostname);

} // namespace tools::net

#endif