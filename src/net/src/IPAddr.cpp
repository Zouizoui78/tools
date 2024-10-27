#include "tools/net/IPAddr.hpp"

#include <cstring>
#include <stdexcept>

namespace tools::net {

IPAddr::IPAddr(const addrinfo *addr) {
    if (addr->ai_addrlen > sizeof(IPAddr)) {
        throw std::runtime_error("Unsupported address type");
    }
    memcpy(this, addr->ai_addr, addr->ai_addrlen);
}

} // namespace tools::net