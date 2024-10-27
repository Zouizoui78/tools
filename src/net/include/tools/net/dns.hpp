#include <netdb.h>
#include <string>
#include <vector>

namespace tools::net {

std::vector<struct addrinfo> dns_lookup(const std::string &hostname);
std::vector<std::string> dns_lookup_str(const std::string &hostname);

} // namespace tools::net