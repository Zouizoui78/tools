#ifndef ADDRESS_FAMILY_HPP
#define ADDRESS_FAMILY_HPP

#include "NetIncludes.hpp"

namespace tools::net {

enum class AddressFamily {
    UNSPEC = AF_UNSPEC,
    INET = AF_INET,
    INET6 = AF_INET6
};

} // namespace tools::net

#endif
