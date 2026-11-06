#pragma once
#include <string>

namespace horizon::renderer {

enum class MenuTab { Legit = 0, Rage, Visuals, Config };

class Menu {
public:
    void initialize();
    void render();
    void toggle() { opened_ = !opened_; }
    bool is_open() const { return opened_; }
    void set_tab(MenuTab t) { tab_ = t; }
private:
    void render_header();
    void render_sidebar();
    void render_auto_hex();
    void render_autobuy();
    void render_maphack();
    void render_camera();
    void render_combos();

    bool opened_{true};
    MenuTab tab_{MenuTab::Legit};
    float anim_{0.f};
};

Menu& menu();

}
