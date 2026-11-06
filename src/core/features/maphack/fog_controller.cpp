#include "fog_controller.hpp"
#include "../../memory/memory.hpp"
#include "../../dota/offsets.hpp"

namespace horizon::features::maphack {

bool FogController::initialize() {
    auto base = memory::mem().base();
    if (!base) return false;
    controller_ = memory::mem().read<uintptr_t>(base + offsets::kFogController);
    return controller_ != 0;
}

void FogController::set_fog_disabled(bool disabled) {
    if (!controller_) return;
    memory::mem().write<bool>(controller_ + offsets::netvars::m_bFogEnabled, !disabled ? true : false);
    if (disabled) {
        memory::mem().write<float>(controller_ + offsets::netvars::m_flFogStart, 0.f);
        memory::mem().write<float>(controller_ + offsets::netvars::m_flFogEnd, 99999.f);
    }
}

void FogController::set_fog_range(float start, float end) {
    if (!controller_) return;
    memory::mem().write<float>(controller_ + offsets::netvars::m_flFogStart, start);
    memory::mem().write<float>(controller_ + offsets::netvars::m_flFogEnd, end);
}

bool FogController::is_fog_disabled() const {
    if (!controller_) return false;
    return !memory::mem().read<bool>(controller_ + offsets::netvars::m_bFogEnabled);
}

}
