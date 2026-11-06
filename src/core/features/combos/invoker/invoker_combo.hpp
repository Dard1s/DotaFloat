#pragma once
#include "../hero_combo_base.hpp"
#include "spell_database.hpp"

namespace horizon::features::combos::invoker {

enum class InvokerComboType { EUL_Cataclysm = 0, Tornado_EMP, ColdSnap_Forged, MaxDamage };

class InvokerCombo : public HeroComboBase {
public:
    const char* hero_name() const override { return "npc_dota_hero_invoker"; }
    void on_update() override;
    bool can_execute(sdk::C_DOTA_BaseNPC* target) const override;
    std::vector<ComboStep> build_sequence(sdk::C_DOTA_BaseNPC* target) const override;
    void set_combo_type(InvokerComboType t) { combo_type_ = t; }
    void set_auto_invoke(bool v) { auto_invoke_ = v; }
private:
    InvokerSpell get_current_orb_spell() const;
    bool invoke_spell(InvokerSpell s) const;
    bool set_orbs(int q, int w, int e) const;
    float get_cataclysm_damage(sdk::C_DOTA_BaseNPC* target) const;
    bool has_aghanim() const;

    InvokerComboType combo_type_{InvokerComboType::EUL_Cataclysm};
    bool auto_invoke_{true};
    int quas_level_{0}, wex_level_{0}, exort_level_{0};
};

InvokerCombo& invoker_combo();

}
