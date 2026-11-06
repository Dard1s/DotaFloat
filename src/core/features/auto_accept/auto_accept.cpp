#include "auto_accept.hpp"
#include "../../memory/memory.hpp"
#include "../../dota/offsets.hpp"
#include "../../dota/game_rules.hpp"
#include <thread>
#include <chrono>

namespace horizon::features::auto_accept {

void AutoAcceptController::on_update() {
    if (!enabled_) return;
    bool found = check_lobby_found();
    float cur = dota::game_rules().get_game_time();
    if (found && !was_found_) {
        state_ = LobbyState::Found;
        found_time_ = cur;
        was_found_ = true;
    }
    if (state_ == LobbyState::Found) {
        float elapsed = (cur - found_time_) * 1000.f;
        if (elapsed >= delay_ms_) {
            do_accept();
            state_ = LobbyState::Accepted;
        }
    }
    if (!found && was_found_) {
        was_found_ = false;
        if (state_ != LobbyState::Accepted) state_ = LobbyState::None;
    }
}

bool AutoAcceptController::check_lobby_found() const {
    uintptr_t mm = get_matchmaking_ptr();
    if (!mm) return false;
    int state = memory::mem().read<int>(mm + 0x28);
    return state == 2;
}

void AutoAcceptController::do_accept() {
    uintptr_t mm = get_matchmaking_ptr();
    if (!mm) return;
    auto fn = memory::mem().get_vfunc(mm, 11);
    (void)fn;
}

uintptr_t AutoAcceptController::get_matchmaking_ptr() const {
    auto base = memory::mem().base();
    if (!base) return 0;
    return memory::mem().read<uintptr_t>(base + offsets::kMatchSignout);
}

AutoAcceptController& auto_accept() { static AutoAcceptController inst; return inst; }

}
