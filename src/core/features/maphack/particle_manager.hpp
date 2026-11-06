#pragma once
#include "../../sdk/vector.hpp"
#include <vector>

namespace horizon::features::maphack {

struct ParticleInfo {
    uintptr_t ptr;
    sdk::Vector3 pos;
    int type;
    float die_time;
};

class ParticleManager {
public:
    bool initialize();
    std::vector<ParticleInfo> get_active_particles() const;
    void suppress_smoke(bool v);
private:
    uintptr_t manager_{0};
};

}
