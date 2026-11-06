#pragma once
#include "c_entity.hpp"
#include "c_dota_item.hpp"
#include <array>

namespace horizon::sdk {

class C_DOTAPlayerController {
public:
    C_DOTA_BaseNPC* get_assigned_hero() const;
    int get_player_id() const;
    int get_team_id() const;
    bool is_local_player() const;
    int get_gold() const;
    int get_unreliable_gold() const;
};

class C_DOTA_CameraController {
public:
    float get_distance() const;
    void set_distance(float desired, bool smooth);
    Vector3 get_target_pos() const;
    void set_target_pos(const Vector3& pos);
};

}
