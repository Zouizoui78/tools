#include "tools/misc/UUID.hpp"

#include <cstring>
#include <format>
#include <stdexcept>

#ifdef _WIN32
#include <Windows.h>
#else
#include <uuid/uuid.h>
#endif

namespace tools {

UUID::UUID() : _uuid({0}) {
    generate();
}

UUID::UUID(const std::string &str) {
    parse(str);
}

const std::array<uint8_t, UUID::length> &UUID::bin() const {
    return this->_uuid;
}

std::string UUID::str() const {
#ifdef _WIN32
    RPC_CSTR str = nullptr;
    const ::UUID *system_uuid =
        reinterpret_cast<const ::UUID *>(this->_uuid.data());

    if (UuidToString(system_uuid, &str) != RPC_S_OK) {
        std::runtime_error("Out of memory");
    }

    std::string ret(reinterpret_cast<char *>(str));
    RpcStringFree(&str);
#else
    std::string ret(UUID_STR_LEN - 1, '\0');
    ::uuid_unparse(this->_uuid.data(), ret.data());
#endif
    return ret;
}

void UUID::generate() {
#ifdef _WIN32
    ::UUID uuid_instance;
    ::UUID *uuid = &uuid_instance;
    auto res = ::UuidCreate(uuid);
    if (res != RPC_S_OK) {
        std::runtime_error("Failed to generate UUID");
    }
#else
    ::uuid_t uuid;
    ::uuid_generate(uuid);
#endif
    memcpy(this->_uuid.data(), uuid, this->_uuid.size());
}

void UUID::parse(const std::string &str) {
    ::uuid_t uuid;
#ifdef _WIN32
    RPC_CSTR cstr = reinterpret_cast<RPC_CSTR>(const_cast<char *>(str.c_str()));
    if (UuidFromString(cstr, &uuid) != RPC_S_OK) {
        throw std::invalid_argument(
            std::format("Invalid uuid string '{}'", str));
    }
    memcpy(this->_uuid.data(), &uuid, this->_uuid.size());
#else
    if (::uuid_parse(str.data(), uuid) != 0) {
        throw std::invalid_argument(
            std::format("Invalid uuid string '{}'", str));
    }
    memcpy(this->_uuid.data(), uuid, this->_uuid.size());
#endif
}

} // namespace tools
