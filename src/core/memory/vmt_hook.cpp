#include "vmt_hook.hpp"
#include <cstring>

namespace horizon::memory {

VmtHook::VmtHook(void* instance) : instance_(instance) {
    if (!instance_) return;
    original_vtable_ = *reinterpret_cast<uintptr_t**>(instance_);
    MEMORY_BASIC_INFORMATION mbi{};
    VirtualQuery(original_vtable_, &mbi, sizeof(mbi));
    vtable_size_ = 0;
    while (original_vtable_[vtable_size_] && vtable_size_ < 512) {
        MEMORY_BASIC_INFORMATION q{};
        if (!VirtualQuery(reinterpret_cast<void*>(original_vtable_[vtable_size_]), &q, sizeof(q))) break;
        if (q.Protect & (PAGE_GUARD | PAGE_NOACCESS)) break;
        ++vtable_size_;
    }
}

VmtHook::~VmtHook() { remove(); }

bool VmtHook::install() {
    if (!instance_ || installed_ || !vtable_size_) return false;
    shadow_vtable_ = std::make_unique<uintptr_t[]>(vtable_size_);
    std::memcpy(shadow_vtable_.get(), original_vtable_, vtable_size_ * sizeof(uintptr_t));
    originals_.assign(original_vtable_, original_vtable_ + vtable_size_);
    *reinterpret_cast<uintptr_t**>(instance_) = shadow_vtable_.get();
    installed_ = true;
    return true;
}

void VmtHook::remove() {
    if (!installed_) return;
    *reinterpret_cast<uintptr_t**>(instance_) = original_vtable_;
    installed_ = false;
}

void* VmtHook::hook_at(int idx, void* detour) {
    if (!installed_ || idx < 0 || static_cast<size_t>(idx) >= vtable_size_) return nullptr;
    void* orig = reinterpret_cast<void*>(shadow_vtable_[idx]);
    shadow_vtable_[idx] = reinterpret_cast<uintptr_t>(detour);
    return orig;
}

void* VmtHook::original_at(int idx) const {
    if (idx < 0 || static_cast<size_t>(idx) >= originals_.size()) return nullptr;
    return reinterpret_cast<void*>(originals_[idx]);
}

}
