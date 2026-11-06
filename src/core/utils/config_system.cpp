#include "config_system.hpp"
#include <fstream>

namespace horizon::utils {

void ConfigSystem::set_defaults() {
    set("auto_hex.enabled", true);
    set("auto_hex.predict_ms", 180);
    set("auto_hex.min_hp", 1200);
    set("autobuy.enabled", true);
    set("autobuy.keep_gold", 150);
    set("auto_accept.enabled", true);
    set("auto_accept.delay_ms", 420);
    set("maphack.enabled", true);
    set("maphack.show_wards", true);
    set("maphack.unhook_particles", true);
    set("camera.zoom_enabled", true);
    set("camera.distance", 1650.f);
    set("combo.invoker.enabled", true);
    set("combo.sf.enabled", true);
    set("combo.tinker.enabled", true);
}

bool ConfigSystem::load(const std::filesystem::path& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f) return false;
    std::string enc((std::istreambuf_iterator<char>(f)), {});
    std::string json = decrypt_json(enc);
    (void)json;
    return true;
}

bool ConfigSystem::save(const std::filesystem::path& path) const {
    std::string json = "{}";
    std::string enc = encrypt_json(json);
    std::ofstream f(path, std::ios::binary);
    if (!f) return false;
    f.write(enc.data(), enc.size());
    return true;
}

std::string ConfigSystem::encrypt_json(const std::string& json) const {
    std::string out = json;
    for (size_t i = 0; i < out.size(); ++i) out[i] ^= static_cast<char>(0x77 + i % 13);
    return out;
}

std::string ConfigSystem::decrypt_json(const std::string& data) const {
    std::string out = data;
    for (size_t i = 0; i < out.size(); ++i) out[i] ^= static_cast<char>(0x77 + i % 13);
    return out;
}

ConfigSystem& config() { static ConfigSystem c; return c; }

}
