#pragma once
#include <cstdint>

namespace horizon::features::maphack {

class FogController {
public:
    bool initialize();
    void set_fog_disabled(bool disabled);
    void set_fog_range(float start, float end);
    bool is_fog_disabled() const;
private:
    uintptr_t controller_{0};
};

}
