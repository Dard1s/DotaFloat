#pragma once
#include <array>
#include <string>

namespace horizon::utils {

template<size_t N, uint8_t K1 = 0xA7, uint8_t K2 = 0x3B>
struct XorStr {
    std::array<char, N> enc;
    constexpr XorStr(const char(&s)[N]) : enc{} {
        for (size_t i = 0; i < N; ++i) enc[i] = s[i] ^ (K1 + static_cast<uint8_t>(i * K2));
    }
    std::string decrypt() const {
        std::string o; o.resize(N-1);
        for (size_t i = 0; i < N-1; ++i) o[i] = enc[i] ^ (K1 + static_cast<uint8_t>(i * K2));
        return o;
    }
};

template<size_t N>
XorStr(const char(&)[N]) -> XorStr<N>;

}
