#ifndef UUID_HPP
#define UUID_HPP

#include <array>
#include <cstdint>
#include <string>

namespace tools {

constexpr int uuid_length = 16;

class UUID {
public:
    // Generate a uuid.
    UUID();

    // Parse the uuid represented by str.
    UUID(const std::string &str);

    const std::array<uint8_t, uuid_length> &bin() const;
    std::string str() const;

    bool operator==(const UUID &other) const = default;

private:
    void generate();
    void parse(const std::string &str);
    std::array<uint8_t, uuid_length> _uuid;
};

} // namespace tools

#endif