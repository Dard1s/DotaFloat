#pragma once
#include "../../shared/common.hpp"
#include <string>
#include <unordered_map>
#include <variant>
#include <filesystem>

namespace horizon::utils {

using ConfigValue = std::variant<bool, int, float, std::string>;

class ConfigSystem {
public:
    bool load(const std::filesystem::path& path);
    bool save(const std::filesystem::path& path) const;
    template<typename T>
    T get(const std::string& key, T def = T{}) const {
        auto it = values_.find(key);
        if (it == values_.end()) return def;
        if (auto* v = std::get_if<T>(&it->second)) return *v;
        return def;
    }
    template<typename T>
    void set(const std::string& key, T val) { values_[key] = val; }
    void set_defaults();
private:
    std::unordered_map<std::string, ConfigValue> values_;
    std::string encrypt_json(const std::string& json) const;
    std::string decrypt_json(const std::string& data) const;
};

ConfigSystem& config();

}
