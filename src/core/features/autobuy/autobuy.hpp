#pragma once
#include "item_database.hpp"
#include <chrono>

namespace horizon::features::autobuy {

class AutobuyController {
public:
    void on_update();
    void set_enabled(bool v) { enabled_ = v; }
    void set_shopping_list(const std::vector<sdk::ItemID>& list) { shopping_list_ = list; }
    void set_keep_gold(int v) { keep_gold_ = v; }
private:
    bool try_purchase_next();
    bool can_shop_now() const;
    int get_current_gold() const;
    void query_shop_stock();

    bool enabled_{true};
    int keep_gold_{150};
    std::vector<sdk::ItemID> shopping_list_{
        sdk::ItemID::Blink, sdk::ItemID::ScytheOfVyse, sdk::ItemID::BKB
    };
    float last_purchase_time_{0.f};
    int pending_cost_{0};
};

AutobuyController& autobuy();

}
