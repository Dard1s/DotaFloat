#pragma once
#include "../sdk/c_entity.hpp"
#include "../sdk/vector.hpp"
#include <vector>

namespace horizon::dota {

class EntitySystem {
public:
    bool initialize();
    sdk::C_DOTA_BaseNPC* get_local_hero() const;
    sdk::C_DOTAPlayerController* get_local_controller() const;
    std::vector<sdk::C_DOTA_BaseNPC*> get_all_heroes() const;
    std::vector<sdk::C_DOTA_BaseNPC*> get_enemies_in_radius(const sdk::Vector3& origin, float radius) const;
    void* get_entity_by_handle(int handle) const;
    sdk::C_DOTA_BaseNPC* get_nearest_enemy(const sdk::Vector3& from, float max_range) const;
private:
    uintptr_t entity_list_{};
    uintptr_t local_player_ptr_{};
};

EntitySystem& entity_system();

}
