#include "entity_system.hpp"
#include "offsets.hpp"
#include "../memory/memory.hpp"

namespace horizon::dota {

bool EntitySystem::initialize() {
    auto base = memory::mem().base();
    if (!base) return false;
    entity_list_ = base + offsets::kEntityList;
    local_player_ptr_ = base + offsets::kLocalPlayer;
    return true;
}

sdk::C_DOTA_BaseNPC* EntitySystem::get_local_hero() const {
    if (!local_player_ptr_) return nullptr;
    uintptr_t local = memory::mem().read<uintptr_t>(local_player_ptr_);
    if (!local) return nullptr;
    uintptr_t hero_handle = memory::mem().read<uintptr_t>(local + offsets::netvars::m_hHero);
    return reinterpret_cast<sdk::C_DOTA_BaseNPC*>(get_entity_by_handle(static_cast<int>(hero_handle)));
}

sdk::C_DOTAPlayerController* EntitySystem::get_local_controller() const {
    if (!local_player_ptr_) return nullptr;
    return reinterpret_cast<sdk::C_DOTAPlayerController*>(memory::mem().read<uintptr_t>(local_player_ptr_));
}

std::vector<sdk::C_DOTA_BaseNPC*> EntitySystem::get_all_heroes() const {
    std::vector<sdk::C_DOTA_BaseNPC*> out;
    if (!entity_list_) return out;
    for (int i = 0; i < 64; ++i) {
        uintptr_t ent = memory::mem().read<uintptr_t>(entity_list_ + i * 0x10);
        if (!ent) continue;
        auto npc = reinterpret_cast<sdk::C_DOTA_BaseNPC*>(ent);
        if (npc->is_hero()) out.push_back(npc);
    }
    return out;
}

std::vector<sdk::C_DOTA_BaseNPC*> EntitySystem::get_enemies_in_radius(const sdk::Vector3& origin, float radius) const {
    std::vector<sdk::C_DOTA_BaseNPC*> result;
    auto heroes = get_all_heroes();
    auto local = get_local_hero();
    int local_team = local ? local->get_team() : 2;
    for (auto* h : heroes) {
        if (h->get_team() == local_team) continue;
        if ((h->get_origin() - origin).length2d() <= radius) result.push_back(h);
    }
    return result;
}

void* EntitySystem::get_entity_by_handle(int handle) const {
    if (!entity_list_) return nullptr;
    int idx = handle & 0xFFF;
    uintptr_t ent = memory::mem().read<uintptr_t>(entity_list_ + idx * 0x10);
    return reinterpret_cast<void*>(ent);
}

sdk::C_DOTA_BaseNPC* EntitySystem::get_nearest_enemy(const sdk::Vector3& from, float max_range) const {
    sdk::C_DOTA_BaseNPC* best = nullptr;
    float best_dist = max_range;
    for (auto* e : get_enemies_in_radius(from, max_range)) {
        float d = (e->get_origin() - from).length();
        if (d < best_dist) { best_dist = d; best = e; }
    }
    return best;
}

EntitySystem& entity_system() { static EntitySystem s; return s; }

}
