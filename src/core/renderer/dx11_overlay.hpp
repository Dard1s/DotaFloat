#pragma once
#include "../sdk/vector.hpp"
#include <d3d11.h>
#include <dxgi.h>
#include <string>

namespace horizon::renderer {

class DX11Overlay {
public:
    bool initialize(HWND hwnd);
    void shutdown();
    void begin_frame();
    void end_frame();
    void draw_text(const sdk::Vector2& pos, const std::string& text, uint32_t color);
    void draw_rect(const sdk::Vector2& a, const sdk::Vector2& b, uint32_t color, bool filled = false);
    void draw_circle(const sdk::Vector2& center, float radius, uint32_t color, int segments = 32);
    void draw_line(const sdk::Vector2& a, const sdk::Vector2& b, uint32_t color, float thickness = 1.f);
    bool world_to_screen(const sdk::Vector3& world, sdk::Vector2& screen) const;
    ID3D11Device* device() const { return device_; }
private:
    HWND hwnd_{};
    ID3D11Device* device_{};
    ID3D11DeviceContext* ctx_{};
    IDXGISwapChain* swapchain_{};
    ID3D11RenderTargetView* rtv_{};
    float screen_w_{1920}, screen_h_{1080};
};

DX11Overlay& overlay();

}
