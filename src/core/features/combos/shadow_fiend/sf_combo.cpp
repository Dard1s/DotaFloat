#include "sf_combo.hpp"
#include "../../../dota/entity_system.hpp"
#include "../../../utils/math_utils.hpp"

namespace horizon::features::combos::sf {

void ShadowFiendCombo::on_update() {
    if (state_ != ComboState::Casting) return;
    if (!current_target_ || !current_target_->is_alive()) { stop(); return; }
    auto seq = build_sequence(current_target_);
    if (step_index_ >= (int)seq.size()) { state_ = ComboState::Finished; return; }
    if (cast_step(seq[step_index_], current_target_)) ++step_index_;
}

bool ShadowFiendCombo::can_execute(sdk::C_DOTA_BaseNPC* target) const {
    if (!target) return false;
    auto* local = dota::entity_system().get_local_hero();
    if (!local) return false;
    float dist = (local->get_origin() - target->get_origin()).length2d();
    return dist < 1000.f && target->health_percent() < 0.85f;
}

std::vector<ComboStep> ShadowFiendCombo::build_sequence(sdk::C_DOTA_BaseNPC* target) const {
    std::vector<ComboStep> seq;
    (void)target;
    if (mode_ == SFComboMode::EulRaze && use_eul_) {
        seq.push_back({"item_cyclone", 0.05f, true, true, 700.f, nullptr});
        seq.push_back({"nevermore_shadowraze1", 2.45f, true, false, 250.f, nullptr});
        seq.push_back({"nevermore_shadowraze2", 0.1f, true, false, 450.f, nullptr});
        seq.push_back({"nevermore_shadowraze3", 0.1f, true, false, 700.f, nullptr});
        seq.push_back({"nevermore_requiem", 0.15f, false, false, 700.f, nullptr});
    } else if (mode_ == SFComboMode::BlinkRaze) {
        seq.push_back({"item_blink", 0.05f, true, true, 1200.f, nullptr});
        seq.push_back({"nevermore_shadowraze3", 0.05f, true, false, 700.f, nullptr});
        seq.push_back({"nevermore_requiem", 0.1f, false, false, 700.f, nullptr});
    } else {
        seq.push_back({"item_invis_sword", 0.05f, false, true, 0.f, nullptr});
        seq.push_back({"nevermore_requiem", 0.2f, false, false, 700.f, nullptr});
    }
    return seq;
}

bool ShadowFiendCombo::is_in_raze_range(const sdk::Vector3& from, const sdk::Vector3& to, int raze_idx) const {
    float dist = (from - to).length2d();
    if (raze_idx == 1) return dist < 250.f;
    if (raze_idx == 2) return dist < 450.f;
    return dist < 700.f;
}

int ShadowFiendCombo::get_best_raze_index(sdk::C_DOTA_BaseNPC* target) const {
    auto* local = dota::entity_system().get_local_hero();
    if (!local || !target) return 3;
    float d = (local->get_origin() - target->get_origin()).length2d();
    if (d < 250) return 1;
    if (d < 450) return 2;
    return 3;
}

float ShadowFiendCombo::get_raze_damage() const { return 350.f; }

ShadowFiendCombo& sf_combo() { static ShadowFiendCombo c; return c; }

}
