#pragma once
#include "vector.hpp"
#include <cstdint>

namespace horizon::sdk {

class C_DOTA_BaseNPC;
class C_DOTABaseAbility;

enum class TeamID : int { Unassigned = 0, Radiant = 2, Dire = 3, Neutral = 4 };
enum class LifeState : uint8_t { Alive = 0, Dying = 1, Dead = 2 };

class C_BaseEntity {
public:
    virtual ~C_BaseEntity() = default;
    Vector3 get_origin() const;
    Vector3 get_abs_origin() const;
    int get_team() const;
    int get_handle_index() const;
    bool is_dormant() const;
    bool is_alive() const;
    int health() const;
    int max_health() const;
    float health_percent() const;
    LifeState life_state() const;
    int handle() const;
    const char* get_classname() const;
private:
    uint8_t pad_0x10[0x18];
};

class C_DOTA_BaseNPC : public C_BaseEntity {
public:
    bool is_hero() const;
    bool is_creep() const;
    bool is_illusion() const;
    bool is_stunned() const;
    bool is_hexed() const;
    bool is_magic_immune() const;
    bool is_invulnerable() const;
    bool is_attackable() const;
    float hull_radius() const;
    float attack_range() const;
    float movement_speed() const;
    int level() const;
    int mana() const;
    int max_mana() const;
    Vector3 get_forward() const;
    C_DOTABaseAbility* get_ability_by_index(int idx) const;
    C_DOTABaseAbility* find_ability_by_name(const char* name) const;
    void* get_modifier_manager() const;
};

class C_DOTABaseAbility {
public:
    virtual ~C_DOTABaseAbility() = default;
    const char* get_name() const;
    int get_level() const;
    float get_cooldown() const;
    float get_cooldown_length() const;
    bool is_ready(float grace) const;
    bool can_be_casted(int mana_cost) const;
    int get_mana_cost() const;
    float get_cast_range() const;
    void* get_owner() const;
};

}
