#pragma once
#include <utility>

namespace horizon::obf {

template<size_t N>
struct xor_string {
    char data[N];
    constexpr xor_string(const char(&str)[N]) : data{} {
        for (size_t i = 0; i < N; ++i) data[i] = str[i] ^ static_cast<char>(0x5A + (i % 7));
    }
    std::string decrypt() const {
        std::string out;
        out.resize(N-1);
        for (size_t i = 0; i < N-1; ++i) out[i] = data[i] ^ static_cast<char>(0x5A + (i % 7));
        return out;
    }
};

template<size_t N>
xor_string(const char(&)[N]) -> xor_string<N>;

#define HORZ_STR(s) (horizon::obf::xor_string(s).decrypt())

inline u64 rotl64(u64 v, int c) { return (v << c) | (v >> (64 - c)); }
inline u64 hash_fnv1a(const char* s) {
    u64 h = 1469598103934665603ull;
    while (*s) { h ^= static_cast<u64>(*s++); h *= 1099511628211ull; }
    return rotl64(h, 13);
}

}
