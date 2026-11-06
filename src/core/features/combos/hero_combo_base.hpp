#pragma once
#include "../../sdk/c_entity.hpp"
#include "../../sdk/vector.hpp"
#include <string>
#include <vector>

namespace horizon::features::combos {

enum class ComboState { Idle, Preparing, Casting, Finished, Interrupted };

struct ComboStep {
    std::string ability_name;
    float delay_before;
    bool require_target;
    bool is_item;
    float cast_range;
    std::function<bool(sdk::C_DOTA_BaseNPC*)> condition;
};

class HeroComboBase {
public:
    virtual ~HeroComboBase() = default;
    virtual const char* hero_name() const = 0;
    virtual void on_update() = 0;
    virtual bool can_execute(sdk::C_DOTA_BaseNPC* target) const = 0;
    virtual std::vector<ComboStep> build_sequence(sdk::C_DOTA_BaseNPC* target) const = 0;
    void execute(sdk::C_DOTA_BaseNPC* target);
    void stop() { state_ = ComboState::Interrupted; step_index_ = 0; }
    ComboState state() const { return state_; }
    bool is_active() const { return state_ == ComboState::Casting || state_ == ComboState::Preparing; }
protected:
    bool cast_step(const ComboStep& step, sdk::C_DOTA_BaseNPC* target);
    sdk::C_DOTABaseAbility* find_ability(const std::string& name) const;
    float get_sequence_dps(const std::vector<ComboStep>& seq, sdk::C_DOTA_BaseNPC* target) const;
    bool check_mana(const std::vector<ComboStep>& seq) const;

    ComboState state_{ComboState::Idle};
    int step_index_{0};
    float sequence_start_{0.f};
    sdk::C_DOTA_BaseNPC* current_target_{nullptr};
};

}
