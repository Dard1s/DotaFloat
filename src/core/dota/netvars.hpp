#pragma once
#include "offsets.hpp"
#include <string>
#include <unordered_map>
#include <optional>

namespace horizon::netvars {

struct NetvarEntry {
    std::string table;
    std::string prop;
    uintptr_t offset;
};

class NetvarManager {
public:
    bool initialize();
    std::optional<uintptr_t> get_offset(const std::string& table, const std::string& prop) const;
    void dump_to_file(const char* path) const;
private:
    std::unordered_map<std::string, uintptr_t> cache_;
    void cache_hardcoded();
};

NetvarManager& netvars();

}
