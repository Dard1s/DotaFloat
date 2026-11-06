#pragma once
#include "../../sdk/c_dota_item.hpp"
#include <vector>
#include <string>
#include <unordered_map>

namespace horizon::features::autobuy {

struct ItemEntry {
    sdk::ItemID id;
    std::string name;
    int base_cost;
    int recipe_cost;
    bool needs_secret_shop;
    int stock_time;
    float winrate_weight;
};

struct PurchasePlan {
    std::vector<sdk::ItemID> queue;
    int total_cost;
    int keep_gold;
};

class ItemDatabase {
public:
    bool initialize();
    const ItemEntry* get_entry(sdk::ItemID id) const;
    int get_real_time_cost(sdk::ItemID id, int game_time) const;
    PurchasePlan generate_plan(const std::vector<sdk::ItemID>& desired, int gold, int game_time) const;
    std::vector<ItemEntry> get_all_entries() const;
private:
    std::unordered_map<int, ItemEntry> entries_;
    void populate_hardcoded();
    float get_inflation_factor(int game_time) const;
};

ItemDatabase& item_db();

}
