#pragma once
#include "../hero_combo_base.hpp"

namespace horizon::features::combos::tinker {

enum class TinkerComboStyle { LaserRocket = 0, RearmSpam, MarchBurst };

class TinkerCombo : public HeroComboBase {
public:
    const char* hero_name() const override { return "npc_dota_hero_tinker"; }
    void on_update() override;
    bool can_execute(sdk::C_DOTA_BaseNPC* target) const override;
    std::vector<ComboStep> build_sequence(sdk::C_DOTA_BaseNPC* target) const override;
    void set_style(TinkerComboStyle s) { style_ = s; }
    void set_auto_rearm(bool v) { auto_rearm_ = v; }
    void set_rearm_threshold(float v) { rearm_threshold_ = v; }
private:
    bool has_blink() const;
    bool has_rearm_ready() const;
    float get_mana_for_full_cycle() const;
    bool should_rearm() const;

    TinkerComboStyle style_{TinkerComboStyle::LaserRocket};
    bool auto_rearm_{true};
    float rearm_threshold_{0.35f};
    int cycles_{0};
};

TinkerCombo& tinker_combo();

}
