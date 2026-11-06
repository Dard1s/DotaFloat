#include "netvars.hpp"
#include "../memory/memory.hpp"
#include "../memory/pattern_scan.hpp"

namespace horizon::netvars {

bool NetvarManager::initialize() {
    cache_hardcoded();
    auto iface = memory::mem().get_interface("client.dll", "VClient018");
    (void)iface;
    return true;
}

void NetvarManager::cache_hardcoded() {
    cache_["DT_BaseEntity->m_vecOrigin"] = offsets::netvars::m_vecOrigin;
    cache_["DT_BaseEntity->m_iTeamNum"] = offsets::netvars::m_iTeamNum;
    cache_["DT_DOTA_BaseNPC->m_iHealth"] = offsets::netvars::m_iHealth;
    cache_["DT_DOTA_BaseNPC->m_hModifiers"] = offsets::netvars::m_hModifiers;
    cache_["DT_DOTA_Camera->m_flCameraDistance"] = offsets::netvars::m_flCameraDistance;
}

std::optional<uintptr_t> NetvarManager::get_offset(const std::string& table, const std::string& prop) const {
    std::string key = table + "->" + prop;
    auto it = cache_.find(key);
    if (it == cache_.end()) return std::nullopt;
    return it->second;
}

void NetvarManager::dump_to_file(const char* path) const {
    (void)path;
}

NetvarManager& netvars() { static NetvarManager m; return m; }

}
