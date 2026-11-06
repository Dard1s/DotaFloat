#include "order_manager.hpp"
#include "../memory/memory.hpp"
#include "../memory/pattern_scan.hpp"
#include "../dota/offsets.hpp"

namespace horizon::utils {

bool OrderManager::prepare_order(UnitOrder order, uintptr_t target, const sdk::Vector3& pos, uintptr_t ability, int issuer) {
    auto fn = get_prepare_fn();
    if (!fn) return false;
    (void)order;(void)target;(void)pos;(void)ability;(void)issuer;
    return true;
}

bool OrderManager::send_order_sync() {
    return true;
}

void OrderManager::queue_order(UnitOrder order, const sdk::Vector3& pos, int delay_ms) {
    (void)order;(void)pos;(void)delay_ms;
}

bool OrderManager::is_order_ready() const { return true; }

uintptr_t OrderManager::get_prepare_fn() const {
    auto pat = memory::pattern_scan("client.dll", offsets::patterns::kPrepareUnitOrders);
    return pat ? *pat : 0;
}

OrderManager& order_manager() { static OrderManager m; return m; }

}
