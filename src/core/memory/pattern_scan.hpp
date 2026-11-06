#pragma once
#include "../../shared/common.hpp"
#include <optional>
#include <string_view>

namespace horizon::memory {

struct Pattern {
    std::string bytes;
    std::string mask;
};

Pattern ida_to_pattern(std::string_view ida);
std::optional<uintptr_t> pattern_scan(const char* module, std::string_view ida_pattern, int offset = 0);
std::optional<uintptr_t> pattern_scan_rel(const char* module, std::string_view pattern, int rip_offset = 3, int rip_size = 7);

template<typename T>
T resolve_rip(uintptr_t addr, int rip_offset = 3) {
    if (!addr) return T{};
    int32_t disp = *reinterpret_cast<int32_t*>(addr + rip_offset);
    return reinterpret_cast<T>(addr + rip_offset + 4 + disp);
}

}
