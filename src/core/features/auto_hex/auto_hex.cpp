#include "auto_hex.hpp"
#include "../../dota/entity_system.hpp"
#include "../../dota/game_rules.hpp"
#include "../../utils/math_utils.hpp"
#include "../../utils/config_system.hpp"

namespace horizon::features::auto_hex {

void AutoHexController::on_update() {
    if (!enabled_) return;
    auto* local = dota::entity_system().get_local_hero();
    if (!local || !local->is_alive()) return;

    auto* hex = get_hex_ability();
    if (!hex || !hex->is_ready(0.1f)) return;

    float range = get_hex_range();
    auto enemies = dota::entity_system().get_enemies_in_radius(local->get_origin(), range + 200.f);
    HexPrediction best{nullptr, 999.f, 9999.f, false};
    for (auto* e : enemies) {
        auto pred = evaluate_target(local, e);
        if (pred.can_hex && pred.distance < best.distance) best = pred;
    }
    if (best.target && best.can_hex) {
        float cur = dota::game_rules().get_game_time();
        if (cur - last_cast_ > 0.25f) {
            if (try_cast_hex(best.target)) last_cast_ = cur;
        }
    }
}

HexPrediction AutoHexController::evaluate_target(sdk::C_DOTA_BaseNPC* local, sdk::C_DOTA_BaseNPC* enemy) {
    HexPrediction p;
    p.target = enemy;
    p.distance = (local->get_origin() - enemy->get_origin()).length2d();
    float range = get_hex_range();
    if (p.distance > range) { p.can_hex = false; return p; }
    if (enemy->is_magic_immune() || enemy->is_invulnerable() || enemy->is_hexed() || enemy->is_stunned()) {
        p.can_hex = false; return p;
    }
    if (enemy->health() < hex_threshold_hp_ && enemy->is_alive()) {
        p.can_hex = should_hex(enemy, p.distance);
    } else {
        p.can_hex = false;
    }
    if (use_prediction_) {
        auto vel = enemy->get_forward() * enemy->movement_speed();
        auto pred_pos = utils::predict_position(enemy->get_origin(), vel, predict_ms_ / 1000.f);
        float pred_dist = (local->get_origin() - pred_pos).length2d();
        p.predicted_time = utils::calc_travel_time(local->get_origin(), pred_pos, 1400.f, get_hex_cast_point());
        p.distance = pred_dist;
        p.can_hex = p.can_hex && pred_dist <= range;
    }
    return p;
}

bool AutoHexController::should_hex(sdk::C_DOTA_BaseNPC* enemy, float dist) {
    (void)dist;
    if (enemy->is_illusion()) return false;
    int hp = enemy->health();
    int mana = enemy->mana();
    if (hp < 600 && mana > 200) return true;
    if (enemy->level() >= 16 && hp < 1500) return true;
    return hp < hex_threshold_hp_;
}

bool AutoHexController::try_cast_hex(sdk::C_DOTA_BaseNPC* target) {
    auto* hex = get_hex_ability();
    if (!hex) return false;
    (void)target;
    return true;
}

float AutoHexController::get_hex_cast_point() const { return 0.1f; }
float AutoHexController::get_hex_range() const { return 700.f; }

sdk::C_DOTABaseAbility* AutoHexController::get_hex_ability() const {
    auto* local = dota::entity_system().get_local_hero();
    if (!local) return nullptr;
    auto* abl = local->find_ability_by_name("item_sheepstick");
    if (abl) return abl;
    abl = local->find_ability_by_name("item_ethereal_blade");
    return abl;
}

AutoHexController& auto_hex() { static AutoHexController inst; return inst; }

}
