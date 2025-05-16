#ifndef UUID_HPP
#define UUID_HPP

#include <array>
#include <cstdint>
#include <string>

namespace tools {

class UUID {
public:
    static const int length = 16;

    // Generate a uuid.
    UUID();

    // Parse the uuid represented by str.
    UUID(const std::string &str);

    const std::array<uint8_t, length> &bin() const;
    std::string str() const;

    bool operator==(const UUID &other) const = default;

private:
    void generate();
    void parse(const std::string &str);
    std::array<uint8_t, length> _uuid;
};

} // namespace tools

#endif
