#include "tinker_combo.hpp"
#include "../../../dota/entity_system.hpp"
#include "../../../dota/game_rules.hpp"

namespace horizon::features::combos::tinker {

void TinkerCombo::on_update() {
    if (state_ != ComboState::Casting) return;
    if (!current_target_ || !current_target_->is_alive()) { if (auto_rearm_ && should_rearm()) { } else { stop(); } return; }
    auto seq = build_sequence(current_target_);
    if (step_index_ >= (int)seq.size()) {
        if (auto_rearm_ && has_rearm_ready() && cycles_ < 6) {
            ++cycles_;
            step_index_ = 0;
            return;
        }
        state_ = ComboState::Finished;
        cycles_ = 0;
        return;
    }
    if (cast_step(seq[step_index_], current_target_)) ++step_index_;
}

bool TinkerCombo::can_execute(sdk::C_DOTA_BaseNPC* target) const {
    if (!target) return false;
    auto* local = dota::entity_system().get_local_hero();
    if (!local) return false;
    if (local->mana() < get_mana_for_full_cycle()) return false;
    float dist = (local->get_origin() - target->get_origin()).length2d();
    return dist < 1200.f;
}

std::vector<ComboStep> TinkerCombo::build_sequence(sdk::C_DOTA_BaseNPC* target) const {
    (void)target;
    std::vector<ComboStep> seq;
    if (style_ == TinkerComboStyle::LaserRocket) {
        seq.push_back({"item_blink", 0.05f, true, true, 1200.f, nullptr});
        seq.push_back({"tinker_laser", 0.1f, true, false, 550.f, nullptr});
        seq.push_back({"tinker_heat_seeking_missile", 0.12f, true, false, 2500.f, nullptr});
        if (has_blink()) seq.push_back({"item_ethereal_blade", 0.08f, true, true, 800.f, nullptr});
        seq.push_back({"tinker_rearm", 2.8f, false, false, 0.f, nullptr});
    } else if (style_ == TinkerComboStyle::MarchBurst) {
        seq.push_back({"tinker_march_of_the_machines", 0.1f, true, false, 700.f, nullptr});
        seq.push_back({"tinker_laser", 0.2f, true, false, 550.f, nullptr});
        seq.push_back({"tinker_rearm", 2.8f, false, false, 0.f, nullptr});
    } else {
        seq.push_back({"tinker_laser", 0.05f, true, false, 550.f, nullptr});
        seq.push_back({"tinker_heat_seeking_missile", 0.1f, true, false, 2500.f, nullptr});
        seq.push_back({"tinker_rearm", 2.8f, false, false, 0.f, nullptr});
    }
    return seq;
}

bool TinkerCombo::has_blink() const {
    auto* local = dota::entity_system().get_local_hero();
    if (!local) return false;
    return local->find_ability_by_name("item_blink") != nullptr;
}

bool TinkerCombo::has_rearm_ready() const {
    auto* local = dota::entity_system().get_local_hero();
    if (!local) return false;
    auto* rearm = local->find_ability_by_name("tinker_rearm");
    return rearm && rearm->is_ready(0.2f);
}

float TinkerCombo::get_mana_for_full_cycle() const { return 450.f; }
bool TinkerCombo::should_rearm() const { return has_rearm_ready(); }

TinkerCombo& tinker_combo() { static TinkerCombo c; return c; }

}
