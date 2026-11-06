#include "economy_tracker.hpp"
#include "../dota/entity_system.hpp"
#include "../dota/game_rules.hpp"

namespace horizon::utils {

void EconomyTracker::on_update() {
    float cur = dota::game_rules().get_game_time();
    if (cur - last_update_ < 0.5f) return;
    auto* ctrl = dota::entity_system().get_local_controller();
    if (!ctrl) return;
    GoldSnapshot s;
    s.gold = ctrl->get_gold();
    s.reliable = ctrl->get_gold();
    s.unreliable = ctrl->get_unreliable_gold();
    s.game_time = cur;
    s.networth = s.gold + 4500;
    history_.push_back(s);
    if (history_.size() > 300) history_.erase(history_.begin());
    last_update_ = cur;
}

int EconomyTracker::get_gpm(int window_seconds) const {
    if (history_.size() < 2) return 0;
    auto now = history_.back();
    auto then = history_.front();
    float dt = now.game_time - then.game_time;
    if (dt <= 0.f) return 0;
    int dg = now.gold - then.gold;
    return static_cast<int>((dg / dt) * 60.f);
}

int EconomyTracker::get_networth() const {
    if (history_.empty()) return 0;
    return history_.back().networth;
}

int EconomyTracker::predict_gold_at(float future_time) const {
    if (history_.empty()) return 0;
    int gpm = get_gpm();
    float cur = dota::game_rules().get_game_time();
    float dt = future_time - cur;
    return history_.back().gold + static_cast<int>(gpm * dt / 60.f);
}

bool EconomyTracker::should_save_for_buyback() const {
    if (history_.empty()) return false;
    return history_.back().game_time > 1800.f;
}

EconomyTracker& economy() { static EconomyTracker e; return e; }

}
