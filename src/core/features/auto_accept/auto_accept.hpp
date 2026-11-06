#pragma once
#include <cstdint>

namespace horizon::features::auto_accept {

enum class LobbyState : int { None = 0, Searching = 1, Found = 2, Accepted = 3, Failed = 4 };

class AutoAcceptController {
public:
    void on_update();
    void set_enabled(bool v) { enabled_ = v; }
    void set_delay_ms(int ms) { delay_ms_ = ms; }
    LobbyState get_state() const { return state_; }
private:
    bool check_lobby_found() const;
    void do_accept();
    uintptr_t get_matchmaking_ptr() const;

    bool enabled_{true};
    int delay_ms_{420};
    LobbyState state_{LobbyState::None};
    float found_time_{0.f};
    bool was_found_{false};
};

AutoAcceptController& auto_accept();

}
