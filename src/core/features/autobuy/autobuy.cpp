#include "autobuy.hpp"
#include "../../dota/entity_system.hpp"
#include "../../dota/game_rules.hpp"
#include "../../dota/offsets.hpp"
#include "../../memory/memory.hpp"

namespace horizon::features::autobuy {

void AutobuyController::on_update() {
    if (!enabled_) return;
    if (!can_shop_now()) return;
    float cur = dota::game_rules().get_game_time();
    if (cur - last_purchase_time_ < 0.5f) return;
    int gold = get_current_gold();
    auto plan = item_db().generate_plan(shopping_list_, gold, static_cast<int>(cur));
    if (plan.queue.empty()) return;
    if (try_purchase_next()) last_purchase_time_ = cur;
}

bool AutobuyController::can_shop_now() const {
    auto* local = dota::entity_system().get_local_hero();
    if (!local || !local->is_alive()) return false;
    return dota::game_rules().is_in_game();
}

int AutobuyController::get_current_gold() const {
    auto* ctrl = dota::entity_system().get_local_controller();
    if (!ctrl) return 0;
    return ctrl->get_gold();
}

bool AutobuyController::try_purchase_next() {
    auto* local = dota::entity_system().get_local_hero();
    if (!local) return false;
    float cur = dota::game_rules().get_game_time();
    int gold = get_current_gold();
    auto plan = item_db().generate_plan(shopping_list_, gold, static_cast<int>(cur));
    if (plan.queue.empty()) return false;
    auto next = plan.queue.front();
    auto* entry = item_db().get_entry(next);
    if (!entry) return false;
    uintptr_t inv = memory::mem().read<uintptr_t>(reinterpret_cast<uintptr_t>(local) + offsets::netvars::m_hItems);
    (void)inv;
    (void)entry;
    return true;
}

void AutobuyController::query_shop_stock() {
    auto base = memory::mem().base();
    uintptr_t shop = memory::mem().read<uintptr_t>(base + offsets::kInventoryManager);
    (void)shop;
}

AutobuyController& autobuy() { static AutobuyController inst; return inst; }

}
