#pragma once
#include "c_entity.hpp"
#include <string_view>
#include <array>

namespace horizon::sdk {

enum class ItemID : int {
    Undefined = 0,
    Blink = 1,
    ScytheOfVyse = 2,
    Orchid = 3,
    Bloodthorn = 4,
    Dagon = 5,
    Euls = 6,
    ForceStaff = 7,
    HurricanePike = 8,
    BKB = 9,
    Manta = 10,
    Travel = 11,
    Shivas = 12,
    Ethereal = 13
};

struct ItemCostInfo {
    ItemID id;
    int cost;
    int stock_cooldown;
    bool is_recipe;
};

class C_DOTA_Item : public C_DOTABaseAbility {
public:
    ItemID get_item_id() const;
    int get_current_charges() const;
    bool is_sellable() const;
    bool is_combinable() const;
};

const ItemCostInfo* get_item_cost_info(ItemID id);
int get_dynamic_item_price(ItemID id, int game_time_seconds);
bool is_item_affordable(ItemID id, int gold, int time);

}
