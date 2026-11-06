#include "maphack.hpp"
#include "fog_controller.hpp"
#include "particle_manager.hpp"
#include "../../memory/memory.hpp"
#include "../../dota/offsets.hpp"
#include "../../dota/entity_system.hpp"
#include "../../dota/game_rules.hpp"

namespace horizon::features::maphack {

void MaphackController::on_update() {
    if (!enabled_) return;
    update_fog_controller();
    float cur = dota::game_rules().get_game_time();
    if (cur - last_scan_ > 0.07f) {
        scan_for_hidden_entities();
        handle_smoke_particles();
        last_scan_ = cur;
    }
}

void MaphackController::update_fog_controller() {
    auto base = memory::mem().base();
    if (!base) return;
    fog_ctrl_ = memory::mem().read<uintptr_t>(base + offsets::kFogController);
    if (!fog_ctrl_) return;
    if (enabled_) {
        memory::mem().write<bool>(fog_ctrl_ + offsets::netvars::m_bFogEnabled, false);
        memory::mem().write<float>(fog_ctrl_ + offsets::netvars::m_flFogStart, 9999.f);
        memory::mem().write<float>(fog_ctrl_ + offsets::netvars::m_flFogEnd, 10000.f);
    } else {
        memory::mem().write<bool>(fog_ctrl_ + offsets::netvars::m_bFogEnabled, true);
    }
}

void MaphackController::scan_for_hidden_entities() {
    auto heroes = dota::entity_system().get_all_heroes();
    for (auto* h : heroes) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(h);
        if (is_entity_visible(ptr)) continue;
        RevealedEntity re;
        re.entity_ptr = ptr;
        re.position = h->get_origin();
        re.team = h->get_team();
        re.is_hero = h->is_hero();
        re.is_ward = false;
        re.last_seen = dota::game_rules().get_game_time();
        revealed_.push_back(re);
        if (revealed_.size() > 128) revealed_.erase(revealed_.begin());
    }
}

void MaphackController::handle_smoke_particles() {
    auto base = memory::mem().base();
    uintptr_t pm = memory::mem().read<uintptr_t>(base + offsets::kParticleManager);
    if (!pm) return;
    int count = memory::mem().read<int>(pm + 0x18);
    for (int i = 0; i < count && i < 2048; ++i) {
        uintptr_t particle = memory::mem().read<uintptr_t>(pm + 0x20 + i * 0x8);
        if (!particle) continue;
        on_particle_create(particle);
    }
}

void MaphackController::on_particle_create(uintptr_t particle_ptr) {
    if (!reveal_wards_) return;
    (void)particle_ptr;
}

bool MaphackController::is_entity_visible(uintptr_t ent) const {
    if (!ent) return false;
    bool dormant = memory::mem().read<bool>(ent + offsets::netvars::m_bIsDormant);
    return !dormant;
}

void MaphackController::spoof_fog(bool state) {
    if (!fog_ctrl_) return;
    memory::mem().write<bool>(fog_ctrl_ + 0x510, state);
}

MaphackController& maphack() { static MaphackController inst; return inst; }

}
