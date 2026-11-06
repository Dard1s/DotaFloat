#include "item_database.hpp"
#include "../../dota/game_rules.hpp"

namespace horizon::features::autobuy {

bool ItemDatabase::initialize() { populate_hardcoded(); return true; }

void ItemDatabase::populate_hardcoded() {
    entries_[(int)sdk::ItemID::ScytheOfVyse] = {sdk::ItemID::ScytheOfVyse, "item_sheepstick", 5650, 0, false, 0, 0.82f};
    entries_[(int)sdk::ItemID::Blink] = {sdk::ItemID::Blink, "item_blink", 2250, 0, false, 0, 0.91f};
    entries_[(int)sdk::ItemID::BKB] = {sdk::ItemID::BKB, "item_black_king_bar", 4050, 0, false, 0, 0.88f};
    entries_[(int)sdk::ItemID::Orchid] = {sdk::ItemID::Orchid, "item_orchid", 3475, 0, false, 0, 0.74f};
    entries_[(int)sdk::ItemID::Dagon] = {sdk::ItemID::Dagon, "item_dagon", 2700, 0, false, 0, 0.65f};
    entries_[(int)sdk::ItemID::Travel] = {sdk::ItemID::Travel, "item_travel_boots", 2500, 0, false, 0, 0.79f};
    entries_[(int)sdk::ItemID::Shivas] = {sdk::ItemID::Shivas, "item_shivas_guard", 5050, 0, false, 0, 0.71f};
}

const ItemEntry* ItemDatabase::get_entry(sdk::ItemID id) const {
    auto it = entries_.find((int)id);
    if (it == entries_.end()) return nullptr;
    return &it->second;
}

float ItemDatabase::get_inflation_factor(int game_time) const {
    if (game_time < 600) return 1.0f;
    if (game_time < 1200) return 1.02f;
    if (game_time < 1800) return 1.05f;
    return 1.08f;
}

int ItemDatabase::get_real_time_cost(sdk::ItemID id, int game_time) const {
    auto* e = get_entry(id);
    if (!e) return 99999;
    float infl = get_inflation_factor(game_time);
    return static_cast<int>(e->base_cost * infl);
}

PurchasePlan ItemDatabase::generate_plan(const std::vector<sdk::ItemID>& desired, int gold, int game_time) const {
    PurchasePlan plan; plan.keep_gold = 150; plan.total_cost = 0;
    for (auto id : desired) {
        int cost = get_real_time_cost(id, game_time);
        if (gold - plan.total_cost >= cost + plan.keep_gold) {
            plan.queue.push_back(id);
            plan.total_cost += cost;
        }
    }
    return plan;
}

std::vector<ItemEntry> ItemDatabase::get_all_entries() const {
    std::vector<ItemEntry> out;
    for (auto& [k,v] : entries_) out.push_back(v);
    return out;
}

ItemDatabase& item_db() { static ItemDatabase db; return db; }

}
