#pragma once
#include <vector>
#include <chrono>

namespace horizon::utils {

struct GoldSnapshot {
    int gold;
    int reliable;
    int unreliable;
    float game_time;
    int networth;
};

class EconomyTracker {
public:
    void on_update();
    int get_gpm(int window_seconds = 60) const;
    int get_networth() const;
    int predict_gold_at(float future_time) const;
    bool should_save_for_buyback() const;
private:
    std::vector<GoldSnapshot> history_;
    float last_update_{0.f};
    int last_gold_{0};
};

EconomyTracker& economy();

}
