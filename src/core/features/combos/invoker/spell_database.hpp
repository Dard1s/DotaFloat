#pragma once
#include <string>
#include <array>
#include <unordered_map>

namespace horizon::features::combos::invoker {

enum class InvokerSpell : int {
    None = 0,
    ColdSnap = 1,
    GhostWalk = 2,
    Tornado = 3,
    EMP = 4,
    Alacrity = 5,
    SunStrike = 6,
    ForgeSpirit = 7,
    IceWall = 8,
    DeafeningBlast = 9,
    Cataclysm = 10
};

struct SpellInfo {
    InvokerSpell id;
    std::string name;
    std::array<int,3> orbs;
    int mana_cost;
    float cooldown;
    float damage;
    float cast_range;
};

class InvokerDatabase {
public:
    const SpellInfo* get_spell(InvokerSpell s) const;
    InvokerSpell get_spell_by_orbs(int quas, int wex, int exort) const;
    std::string orbs_to_string(int q, int w, int e) const;
private:
    std::unordered_map<int, SpellInfo> spells_;
    void init();
};

}
