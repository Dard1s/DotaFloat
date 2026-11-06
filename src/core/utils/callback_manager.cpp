#include "callback_manager.hpp"

namespace horizon::utils {

void CallbackManager::dispatch_update(float dt) {
    for (auto& cb : update_cbs_) cb(dt);
}

void CallbackManager::dispatch_render(float dt) {
    for (auto& cb : render_cbs_) cb(dt);
}

CallbackManager& callbacks() { static CallbackManager m; return m; }

}
