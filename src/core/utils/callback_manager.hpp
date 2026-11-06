#pragma once
#include <functional>
#include <vector>

namespace horizon::utils {

using FrameCallback = std::function<void(float)>;

class CallbackManager {
public:
    void add_on_update(FrameCallback cb) { update_cbs_.push_back(cb); }
    void add_on_render(FrameCallback cb) { render_cbs_.push_back(cb); }
    void dispatch_update(float dt);
    void dispatch_render(float dt);
private:
    std::vector<FrameCallback> update_cbs_;
    std::vector<FrameCallback> render_cbs_;
};

CallbackManager& callbacks();

}
