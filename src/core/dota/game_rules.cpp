#include "game_rules.hpp"
#include "offsets.hpp"
#include "../memory/memory.hpp"

namespace horizon::dota {

bool GameRules::initialize() {
    auto base = memory::mem().base();
    if (!base) return false;
    game_rules_ = memory::mem().read<uintptr_t>(base + offsets::kGameRules);
    return game_rules_ != 0;
}

float GameRules::get_game_time() const {
    if (!game_rules_) return 0.f;
    return memory::mem().read<float>(game_rules_ + 0x58);
}

float GameRules::get_pre_game_time() const {
    if (!game_rules_) return 0.f;
    return memory::mem().read<float>(game_rules_ + 0x5C);
}

int GameRules::get_game_mode() const {
    if (!game_rules_) return 0;
    return memory::mem().read<int>(game_rules_ + 0x34);
}

GameState GameRules::get_game_state() const {
    if (!game_rules_) return GameState::None;
    return static_cast<GameState>(memory::mem().read<int>(game_rules_ + 0x30));
}

bool GameRules::is_in_game() const {
    return get_game_state() == GameState::InProgress;
}

bool GameRules::is_daytime() const {
    if (!game_rules_) return true;
    return memory::mem().read<bool>(game_rules_ + 0x88);
}

float GameRules::get_day_night_cycle() const {
    if (!game_rules_) return 0.f;
    return memory::mem().read<float>(game_rules_ + 0x8C);
}

int GameRules::get_ros_respawn_time() const {
    if (!game_rules_) return 0;
    return memory::mem().read<int>(game_rules_ + 0xA0);
}

GameRules& game_rules() { static GameRules g; return g; }

}
