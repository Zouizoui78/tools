#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include <ranges>
#include <type_traits>

namespace tools::endian {

template <typename T>
    requires std::is_fundamental_v<T>
T swap(T in) {
    T swapped = 0;
    char *swapped_as_char = reinterpret_cast<char *>(&swapped);
    char *in_as_char = reinterpret_cast<char *>(&in);
    auto type_size = sizeof(T);

    for (int i = 0; i < type_size; i++) {
        swapped_as_char[i] = in_as_char[type_size - i - 1];
    }
    return swapped;
}

template <typename R>
    requires std::ranges::input_range<R> &&
             std::is_fundamental_v<std::ranges::range_value_t<R>>
R swap_range(R in) {
    R out{in};
    for (auto &e : out) {
        e = swap(e);
    }
    return out;
}

} // namespace tools::endian

#endif // ENDIAN_HPP