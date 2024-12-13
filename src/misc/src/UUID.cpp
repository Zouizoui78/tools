#include "tools/misc/UUID.hpp"

#include <cstring>
#include <format>
#include <stdexcept>
#include <uuid/uuid.h>

namespace tools {

UUID::UUID() : _uuid({0}) {
    generate();
}

UUID::UUID(const std::string &str) {
    ::uuid_t uuid;
    if (::uuid_parse(str.data(), uuid) != 0) {
        throw std::invalid_argument(
            std::format("Failed to parse uuid '{}'", str));
    }
    memcpy(this->_uuid.data(), uuid, this->_uuid.size());
}

tools::uuid_t UUID::bin() const {
    return this->_uuid;
}

std::string UUID::str() const {
    std::string ret;
    ret.resize(UUID_STR_LEN - 1);
    ::uuid_unparse(this->_uuid.data(), ret.data());
    return ret;
}

void UUID::generate() {
    ::uuid_t uuid;
    ::uuid_generate(uuid);
    memcpy(this->_uuid.data(), uuid, this->_uuid.size());
}

} // namespace tools