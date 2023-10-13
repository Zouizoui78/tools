#ifndef ENDIAN_HPP
#define ENDIAN_HPP

namespace tools::endian {

template <typename T> T swap_bytes(T in) {
    T swapped = 0;
    char* swapped_as_char = reinterpret_cast<char*>(&swapped);
    char* in_as_char = reinterpret_cast<char*>(&in);
    auto type_size = sizeof(T);

    for (int i = 0; i < type_size; i++) {
        swapped_as_char[i] = in_as_char[type_size - i - 1];
    }
    return swapped;
}

} // namespace tools::endian

#endif // ENDIAN_HPP