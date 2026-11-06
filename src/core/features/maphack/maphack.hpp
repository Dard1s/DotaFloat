#pragma once
#include "../../sdk/vector.hpp"
#include <vector>

namespace horizon::features::maphack {

struct RevealedEntity {
    uintptr_t entity_ptr;
    sdk::Vector3 position;
    int team;
    float last_seen;
    bool is_ward;
    bool is_hero;
};

class MaphackController {
public:
    void on_update();
    void on_particle_create(uintptr_t particle_ptr);
    void set_enabled(bool v) { enabled_ = v; }
    void set_reveal_wards(bool v) { reveal_wards_ = v; }
    const std::vector<RevealedEntity>& get_revealed() const { return revealed_; }
private:
    void update_fog_controller();
    void scan_for_hidden_entities();
    void handle_smoke_particles();
    bool is_entity_visible(uintptr_t ent) const;
    void spoof_fog(bool state);

    bool enabled_{true};
    bool reveal_wards_{true};
    bool unhook_particles_{true};
    std::vector<RevealedEntity> revealed_;
    float last_scan_{0.f};
    uintptr_t fog_ctrl_{0};
};

MaphackController& maphack();

}
