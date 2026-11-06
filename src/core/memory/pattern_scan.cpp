#include "pattern_scan.hpp"
#include "memory.hpp"
#include <windows.h>
#include <psapi.h>

namespace horizon::memory {

Pattern ida_to_pattern(std::string_view ida) {
    Pattern p;
    for (size_t i = 0; i < ida.size(); ) {
        if (ida[i] == ' ') { ++i; continue; }
        if (ida[i] == '?') {
            p.bytes.push_back('\x00');
            p.mask.push_back('?');
            if (i+1 < ida.size() && ida[i+1]=='?') ++i;
            ++i;
        } else {
            char hex[3]={static_cast<char>(ida[i]), static_cast<char>(ida[i+1]), 0};
            p.bytes.push_back(static_cast<char>(strtol(hex,nullptr,16)));
            p.mask.push_back('x');
            i+=2;
        }
    }
    return p;
}

std::optional<uintptr_t> pattern_scan(const char* module, std::string_view ida_pattern, int offset) {
    auto pat = ida_to_pattern(ida_pattern);
    HMODULE mod = GetModuleHandleA(module);
    if (!mod) return std::nullopt;
    MODULEINFO mi{};
    GetModuleInformation(GetCurrentProcess(), mod, &mi, sizeof(mi));
    auto base = reinterpret_cast<uint8_t*>(mi.lpBaseOfDll);
    size_t size = mi.SizeOfImage;
    for (size_t i = 0; i < size - pat.bytes.size(); ++i) {
        bool found = true;
        for (size_t j = 0; j < pat.bytes.size(); ++j) {
            if (pat.mask[j]=='x' && base[i+j]!=static_cast<uint8_t>(pat.bytes[j])) { found=false; break; }
        }
        if (found) return reinterpret_cast<uintptr_t>(base + i + offset);
    }
    return std::nullopt;
}

std::optional<uintptr_t> pattern_scan_rel(const char* module, std::string_view pattern, int rip_offset, int rip_size) {
    auto addr = pattern_scan(module, pattern, 0);
    if (!addr) return std::nullopt;
    int32_t disp = *reinterpret_cast<int32_t*>(*addr + rip_offset);
    return *addr + rip_size + disp;
}

}
