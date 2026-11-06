#pragma once
#include "../hero_combo_base.hpp"

namespace horizon::features::combos::sf {

enum class SFComboMode { EulRaze = 0, BlinkRaze, RequiemBurst };

class ShadowFiendCombo : public HeroComboBase {
public:
    const char* hero_name() const override { return "npc_dota_hero_nevermore"; }
    void on_update() override;
    bool can_execute(sdk::C_DOTA_BaseNPC* target) const override;
    std::vector<ComboStep> build_sequence(sdk::C_DOTA_BaseNPC* target) const override;
    void set_mode(SFComboMode m) { mode_ = m; }
    void set_use_eul(bool v) { use_eul_ = v; }
    float get_raze_damage() const;
private:
    bool is_in_raze_range(const sdk::Vector3& from, const sdk::Vector3& to, int raze_idx) const;
    int get_best_raze_index(sdk::C_DOTA_BaseNPC* target) const;
    SFComboMode mode_{SFComboMode::EulRaze};
    bool use_eul_{true};
    float eul_delay_{2.5f};
};

ShadowFiendCombo& sf_combo();

}
