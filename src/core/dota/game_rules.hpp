#pragma once
#include <cstdint>

namespace horizon::dota {

enum class GameState : int {
    None = 0,
    PreGame = 1,
    StrategyTime = 2,
    Draft = 3,
    InProgress = 4,
    PostGame = 5
};

class GameRules {
public:
    bool initialize();
    float get_game_time() const;
    float get_pre_game_time() const;
    int get_game_mode() const;
    GameState get_game_state() const;
    bool is_in_game() const;
    bool is_daytime() const;
    float get_day_night_cycle() const;
    int get_ros_respawn_time() const;
private:
    uintptr_t game_rules_{};
};

GameRules& game_rules();

}
