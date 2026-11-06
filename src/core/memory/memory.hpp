#pragma once
#include "../../shared/common.hpp"
#include <windows.h>
#include <optional>

namespace horizon::memory {

class MemoryManager {
public:
    bool attach(const char* module_name);
    bool is_attached() const { return base_ != 0; }
    uintptr_t base() const { return base_; }
    uintptr_t get_module_base(const char* name) const;
    uint32_t get_module_size(const char* name) const;

    template<typename T>
    T read(uintptr_t addr) const {
        if (!addr) return T{};
        return *reinterpret_cast<T*>(addr);
    }

    template<typename T>
    bool write(uintptr_t addr, const T& value) const {
        if (!addr) return false;
        *reinterpret_cast<T*>(addr) = value;
        return true;
    }

    bool read_raw(uintptr_t addr, void* out, size_t size) const;
    bool write_raw(uintptr_t addr, const void* data, size_t size) const;
    bool protect(uintptr_t addr, size_t size, DWORD prot, DWORD* old) const;

    uintptr_t get_interface(const char* module, const char* iface) const;
    uintptr_t get_vfunc(uintptr_t instance, int idx) const;

private:
    uintptr_t base_{};
    HMODULE cached_client_{};
};

MemoryManager& mem();

}
