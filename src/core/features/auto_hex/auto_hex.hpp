#pragma once
#include "../../sdk/c_entity.hpp"
#include <chrono>

namespace horizon::features::auto_hex {

struct HexPrediction {
    sdk::C_DOTA_BaseNPC* target{};
    float predicted_time{};
    float distance{};
    bool can_hex{};
};

class AutoHexController {
public:
    void on_update();
    void set_enabled(bool v) { enabled_ = v; }
    bool is_enabled() const { return enabled_; }
    void set_predict_ms(int ms) { predict_ms_ = ms; }
    void set_use_prediction(bool v) { use_prediction_ = v; }
private:
    HexPrediction evaluate_target(sdk::C_DOTA_BaseNPC* local, sdk::C_DOTA_BaseNPC* enemy);
    bool should_hex(sdk::C_DOTA_BaseNPC* enemy, float dist);
    bool try_cast_hex(sdk::C_DOTA_BaseNPC* target);
    float get_hex_cast_point() const;
    float get_hex_range() const;
    sdk::C_DOTABaseAbility* get_hex_ability() const;

    bool enabled_{true};
    bool use_prediction_{true};
    int predict_ms_{180};
    float last_cast_{0.f};
    int hex_threshold_hp_{1200};
};

AutoHexController& auto_hex();

}
