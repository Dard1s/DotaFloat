#include "zoom_controller.hpp"
#include "../../memory/memory.hpp"
#include "../../dota/offsets.hpp"
#include "../../utils/config_system.hpp"

namespace horizon::features::camera {

bool ZoomController::initialize() {
    auto base = memory::mem().base();
    camera_mgr_ = memory::mem().read<uintptr_t>(base + offsets::kCameraManager);
    patch_convar_limit();
    return camera_mgr_ != 0;
}

void ZoomController::on_update() {
    if (!enabled_) {
        write_distance(default_distance_);
        return;
    }
    float cur = read_current_distance();
    if (smooth_) {
        float lerp = cur + (desired_distance_ - cur) * 0.15f;
        write_distance(lerp);
    } else {
        write_distance(desired_distance_);
    }
}

float ZoomController::read_current_distance() const {
    if (!camera_mgr_) return default_distance_;
    return memory::mem().read<float>(camera_mgr_ + offsets::netvars::m_flCameraDistance);
}

void ZoomController::write_distance(float v) {
    if (!camera_mgr_) return;
    memory::mem().write<float>(camera_mgr_ + offsets::netvars::m_flCameraDistance, v);
    auto convar = memory::mem().read<uintptr_t>(memory::mem().base() + offsets::kConVarBase);
    if (convar) memory::mem().write<float>(convar + offsets::convars::dota_camera_distance, v);
}

void ZoomController::patch_convar_limit() {
    if (patched_) return;
    auto base = memory::mem().base();
    uintptr_t convar = memory::mem().read<uintptr_t>(base + offsets::kConVarBase);
    if (!convar) return;
    float max_limit = 2500.f;
    memory::mem().write<float>(convar + 0x48, max_limit);
    patched_ = true;
}

ZoomController& zoom_controller() { static ZoomController z; return z; }

}
