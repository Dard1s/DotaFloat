#include "memory/memory.hpp"
#include "dota/entity_system.hpp"
#include "dota/game_rules.hpp"
#include "dota/netvars.hpp"
#include "features/auto_hex/auto_hex.hpp"
#include "features/autobuy/autobuy.hpp"
#include "features/auto_accept/auto_accept.hpp"
#include "features/maphack/maphack.hpp"
#include "features/camera/zoom_controller.hpp"
#include "features/combos/invoker/invoker_combo.hpp"
#include "features/combos/shadow_fiend/sf_combo.hpp"
#include "features/combos/tinker/tinker_combo.hpp"
#include "renderer/dx11_overlay.hpp"
#include "renderer/menu.hpp"
#include "utils/config_system.hpp"
#include "utils/logger.hpp"
#include <windows.h>
#include <thread>

using namespace horizon;

DWORD WINAPI DotaFloatThread(LPVOID) {
    utils::config().set_defaults();
    utils::config().load("dotafloat.cfg");

    if (!memory::mem().attach("client.dll")) {
        HORZ_LOG_ERR("failed to attach client");
        return 0;
    }

    netvars::netvars().initialize();
    dota::entity_system().initialize();
    dota::game_rules().initialize();
    features::autobuy::item_db().initialize();
    features::camera::zoom_controller().initialize();

    renderer::overlay().initialize(FindWindowA(nullptr, "Dota 2"));
    renderer::menu().initialize();

    HORZ_LOG_INFO("DotaFloat injected v{}", 0x024A1B);

    while (true) {
        if (GetAsyncKeyState(VK_INSERT) & 1) renderer::menu().toggle();
        if (GetAsyncKeyState(VK_END) & 0x8000) break;

        features::auto_hex::auto_hex().on_update();
        features::autobuy::autobuy().on_update();
        features::auto_accept::auto_accept().on_update();
        features::maphack::maphack().on_update();
        features::camera::zoom_controller().on_update();
        features::combos::invoker::invoker_combo().on_update();
        features::combos::sf::sf_combo().on_update();
        features::combos::tinker::tinker_combo().on_update();

        renderer::overlay().begin_frame();
        renderer::menu().render();
        renderer::overlay().end_frame();

        std::this_thread::sleep_for(std::chrono::milliseconds(6));
    }

    renderer::overlay().shutdown();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(mod);
        CreateThread(nullptr, 0, DotaFloatThread, nullptr, 0, nullptr);
    }
    return TRUE;
}
