#include "particle_manager.hpp"
#include "../../memory/memory.hpp"
#include "../../dota/offsets.hpp"

namespace horizon::features::maphack {

bool ParticleManager::initialize() {
    auto base = memory::mem().base();
    manager_ = memory::mem().read<uintptr_t>(base + offsets::kParticleManager);
    return manager_ != 0;
}

std::vector<ParticleInfo> ParticleManager::get_active_particles() const {
    std::vector<ParticleInfo> out;
    if (!manager_) return out;
    int count = memory::mem().read<int>(manager_ + 0x18);
    for (int i = 0; i < count && i < 1024; ++i) {
        uintptr_t p = memory::mem().read<uintptr_t>(manager_ + 0x20 + i * 0x8);
        if (!p) continue;
        ParticleInfo info;
        info.ptr = p;
        info.pos = memory::mem().read<sdk::Vector3>(p + 0x10);
        info.type = memory::mem().read<int>(p + 0x30);
        out.push_back(info);
    }
    return out;
}

void ParticleManager::suppress_smoke(bool v) {
    (void)v;
}

}
