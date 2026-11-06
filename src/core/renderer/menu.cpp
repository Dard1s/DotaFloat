#include "menu.hpp"
#include "dx11_overlay.hpp"
#include "../utils/config_system.hpp"

namespace horizon::renderer {

void Menu::initialize() { opened_ = true; }
void Menu::render() {
    if (!opened_) return;
    render_header();
    render_sidebar();
    if (tab_ == MenuTab::Legit) {
        render_auto_hex();
        render_autobuy();
    } else if (tab_ == MenuTab::Visuals) {
        render_maphack();
        render_camera();
    } else if (tab_ == MenuTab::Rage) {
        render_combos();
    }
}
void Menu::render_header() { overlay().draw_rect({20,20},{640,60}, 0xFF15151A, true); }
void Menu::render_sidebar() { overlay().draw_rect({20,60},{180,580}, 0xFF1E1E24, true); }
void Menu::render_auto_hex() { overlay().draw_text({200,80}, "Auto Hex", 0xFFFFFFFF); }
void Menu::render_autobuy() { overlay().draw_text({200,120}, "Autobuy", 0xFFFFFFFF); }
void Menu::render_maphack() { overlay().draw_text({200,80}, "Maphack - Fog Reveal", 0xFFFFFFFF); }
void Menu::render_camera() { overlay().draw_text({200,120}, "Zoom Controller", 0xFFFFFFFF); }
void Menu::render_combos() { overlay().draw_text({200,80}, "Invoker / SF / Tinker Combos", 0xFFFFFFFF); }
Menu& menu() { static Menu m; return m; }

}
