#pragma once

namespace horizon::features::camera {

class ZoomController {
public:
    bool initialize();
    void on_update();
    void set_enabled(bool v) { enabled_ = v; }
    void set_distance(float d) { desired_distance_ = d; }
    float get_distance() const { return desired_distance_; }
    void set_smooth(bool v) { smooth_ = v; }
private:
    float read_current_distance() const;
    void write_distance(float v);
    void patch_convar_limit();

    bool enabled_{true};
    float desired_distance_{1650.f};
    float default_distance_{1134.f};
    bool smooth_{true};
    bool patched_{false};
    uintptr_t camera_mgr_{0};
};

ZoomController& zoom_controller();

}
