#pragma once
#include "../sdk/vector.hpp"
#include <cstdint>

namespace horizon::utils {

enum class UnitOrder : int {
    None = 0,
    MoveTo = 1,
    AttackTarget = 4,
    CastTarget = 6,
    CastPosition = 7,
    CastNoTarget = 8,
    BuyItem = 16,
    SellItem = 17
};

class OrderManager {
public:
    bool prepare_order(UnitOrder order, uintptr_t target, const sdk::Vector3& pos, uintptr_t ability, int issuer);
    bool send_order_sync();
    void queue_order(UnitOrder order, const sdk::Vector3& pos, int delay_ms);
    bool is_order_ready() const;
private:
    uintptr_t get_prepare_fn() const;
};

OrderManager& order_manager();

}
