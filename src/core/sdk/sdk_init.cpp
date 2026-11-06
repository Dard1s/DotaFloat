#include "sdk_init.hpp"
#include "../memory/pattern_scan.hpp"
#include "../dota/offsets.hpp"
#include "../memory/memory.hpp"

namespace horizon::sdk {

bool initialize_sdk() {
    auto res = memory::pattern_scan("client.dll", offsets::patterns::kEntitySystem);
    if (!res) return false;
    auto w2s = memory::pattern_scan("client.dll", offsets::patterns::kWorldToScreen);
    (void)w2s;
    return true;
}

void shutdown_sdk() {}

}
