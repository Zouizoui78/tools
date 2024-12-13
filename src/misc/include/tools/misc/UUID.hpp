#ifndef UUID_HPP
#define UUID_HPP

#include <array>
#include <cstdint>
#include <string>

namespace tools {

using uuid_t = std::array<uint8_t, 16>;

class UUID {
public:
    // Generate a uuid.
    UUID();

    // Parse the uuid represented by str.
    UUID(const std::string &str);

    uuid_t bin() const;
    std::string str() const;

    bool operator==(const UUID &other) const = default;

private:
    void generate();
    uuid_t _uuid;
};

} // namespace tools

#endif