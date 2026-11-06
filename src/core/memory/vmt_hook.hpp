#pragma once
#include "../../shared/common.hpp"
#include <windows.h>
#include <vector>
#include <memory>

namespace horizon::memory {

class VmtHook {
public:
    explicit VmtHook(void* instance);
    ~VmtHook();
    bool install();
    void remove();
    void* hook_at(int idx, void* detour);
    void* original_at(int idx) const;
    template<typename T>
    T get_original(int idx) const { return reinterpret_cast<T>(original_at(idx)); }
private:
    void* instance_{};
    uintptr_t* original_vtable_{};
    std::unique_ptr<uintptr_t[]> shadow_vtable_{};
    size_t vtable_size_{};
    std::vector<uintptr_t> originals_{};
    bool installed_{};
};

}
