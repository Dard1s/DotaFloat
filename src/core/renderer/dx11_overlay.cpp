#include "dx11_overlay.hpp"
#include "../memory/memory.hpp"
#include "../dota/offsets.hpp"

namespace horizon::renderer {

bool DX11Overlay::initialize(HWND hwnd) {
    hwnd_ = hwnd;
    DXGI_SWAP_CHAIN_DESC desc{};
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hwnd_;
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    D3D_FEATURE_LEVEL lvl;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &desc, &swapchain_, &device_, &lvl, &ctx_);
    return SUCCEEDED(hr);
}

void DX11Overlay::shutdown() {
    if (rtv_) rtv_->Release();
    if (swapchain_) swapchain_->Release();
    if (ctx_) ctx_->Release();
    if (device_) device_->Release();
}

void DX11Overlay::begin_frame() {
    float clear[4] = {0.02f, 0.02f, 0.025f, 1.f};
    if (ctx_ && rtv_) ctx_->ClearRenderTargetView(rtv_, clear);
}

void DX11Overlay::end_frame() {
    if (swapchain_) swapchain_->Present(1, 0);
}

void DX11Overlay::draw_text(const sdk::Vector2& pos, const std::string& text, uint32_t color) { (void)pos;(void)text;(void)color; }
void DX11Overlay::draw_rect(const sdk::Vector2& a, const sdk::Vector2& b, uint32_t color, bool filled) { (void)a;(void)b;(void)color;(void)filled; }
void DX11Overlay::draw_circle(const sdk::Vector2& center, float radius, uint32_t color, int segments) { (void)center;(void)radius;(void)color;(void)segments; }
void DX11Overlay::draw_line(const sdk::Vector2& a, const sdk::Vector2& b, uint32_t color, float thickness) { (void)a;(void)b;(void)color;(void)thickness; }

bool DX11Overlay::world_to_screen(const sdk::Vector3& world, sdk::Vector2& screen) const {
    auto base = memory::mem().base();
    if (!base) return false;
    auto vm_ptr = memory::mem().read<uintptr_t>(base + offsets::kViewMatrix);
    if (!vm_ptr) return false;
    sdk::ViewMatrix vm = memory::mem().read<sdk::ViewMatrix>(vm_ptr);
    return vm.world_to_screen(world, screen, screen_w_, screen_h_);
}

DX11Overlay& overlay() { static DX11Overlay o; return o; }

}
