#include "invoker_combo.hpp"
#include "../../../dota/entity_system.hpp"
#include "../../../dota/game_rules.hpp"
#include "../../../utils/math_utils.hpp"

namespace horizon::features::combos::invoker {

void InvokerCombo::on_update() {
    if (state_ != ComboState::Casting) return;
    if (!current_target_ || !current_target_->is_alive()) { stop(); return; }
    auto seq = build_sequence(current_target_);
    if (step_index_ >= (int)seq.size()) { state_ = ComboState::Finished; return; }
    auto& step = seq[step_index_];
    if (cast_step(step, current_target_)) ++step_index_;
}

bool InvokerCombo::can_execute(sdk::C_DOTA_BaseNPC* target) const {
    if (!target || !target->is_alive()) return false;
    auto* local = dota::entity_system().get_local_hero();
    if (!local) return false;
    float dist = (local->get_origin() - target->get_origin()).length2d();
    if (dist > 1200.f) return false;
    auto seq = build_sequence(target);
    return check_mana(seq);
}

std::vector<ComboStep> InvokerCombo::build_sequence(sdk::C_DOTA_BaseNPC* target) const {
    (void)target;
    std::vector<ComboStep> seq;
    if (combo_type_ == InvokerComboType::EUL_Cataclysm) {
        seq.push_back({"item_cyclone", 0.05f, true, true, 700.f, nullptr});
        seq.push_back({"invoker_sun_strike", 0.1f, true, false, 1800.f, nullptr});
        seq.push_back({"invoker_cataclysm", 1.7f, false, false, 0.f, nullptr});
        seq.push_back({"invoker_deafening_blast", 0.2f, true, false, 1000.f, nullptr});
    } else if (combo_type_ == InvokerComboType::Tornado_EMP) {
        seq.push_back({"invoker_tornado", 0.05f, true, false, 2000.f, nullptr});
        seq.push_back({"invoker_emp", 0.3f, true, false, 950.f, nullptr});
        seq.push_back({"invoker_ice_wall", 0.2f, true, false, 800.f, nullptr});
    } else {
        seq.push_back({"invoker_cold_snap", 0.05f, true, false, 1000.f, nullptr});
        seq.push_back({"invoker_forge_spirit", 0.15f, false, false, 0.f, nullptr});
        seq.push_back({"invoker_alacrity", 0.1f, true, false, 700.f, nullptr});
    }
    return seq;
}

InvokerSpell InvokerCombo::get_current_orb_spell() const { return InvokerSpell::Cataclysm; }
bool InvokerCombo::invoke_spell(InvokerSpell s) const { (void)s; return true; }
bool InvokerCombo::set_orbs(int q, int w, int e) const { (void)q;(void)w;(void)e; return true; }
float InvokerCombo::get_cataclysm_damage(sdk::C_DOTA_BaseNPC* target) const { (void)target; return 1600.f; }
bool InvokerCombo::has_aghanim() const { return true; }

InvokerCombo& invoker_combo() { static InvokerCombo c; return c; }

}
