#include "memory.hpp"
#include "../../shared/obfuscation.hpp"
#include <psapi.h>

namespace horizon::memory {

bool MemoryManager::attach(const char* module_name) {
    cached_client_ = GetModuleHandleA(module_name);
    if (!cached_client_) return false;
    base_ = reinterpret_cast<uintptr_t>(cached_client_);
    return true;
}

uintptr_t MemoryManager::get_module_base(const char* name) const {
    HMODULE mod = GetModuleHandleA(name);
    return reinterpret_cast<uintptr_t>(mod);
}

uint32_t MemoryManager::get_module_size(const char* name) const {
    HMODULE mod = GetModuleHandleA(name);
    if (!mod) return 0;
    auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(mod);
    auto nt = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(mod) + dos->e_lfanew);
    return nt->OptionalHeader.SizeOfImage;
}

bool MemoryManager::read_raw(uintptr_t addr, void* out, size_t size) const {
    if (!addr || !out) return false;
    std::memcpy(out, reinterpret_cast<void*>(addr), size);
    return true;
}

bool MemoryManager::write_raw(uintptr_t addr, const void* data, size_t size) const {
    if (!addr || !data) return false;
    DWORD old{};
    VirtualProtect(reinterpret_cast<void*>(addr), size, PAGE_EXECUTE_READWRITE, &old);
    std::memcpy(reinterpret_cast<void*>(addr), data, size);
    VirtualProtect(reinterpret_cast<void*>(addr), size, old, &old);
    return true;
}

bool MemoryManager::protect(uintptr_t addr, size_t size, DWORD prot, DWORD* old) const {
    return VirtualProtect(reinterpret_cast<void*>(addr), size, prot, old) != 0;
}

uintptr_t MemoryManager::get_interface(const char* module, const char* iface) const {
    HMODULE mod = GetModuleHandleA(module);
    if (!mod) return 0;
    auto fn = reinterpret_cast<uintptr_t(*)(const char*, int*)>(GetProcAddress(mod, "CreateInterface"));
    if (!fn) return 0;
    return fn(iface, nullptr);
}

uintptr_t MemoryManager::get_vfunc(uintptr_t instance, int idx) const {
    if (!instance) return 0;
    auto vtable = *reinterpret_cast<uintptr_t*>(instance);
    if (!vtable) return 0;
    return *reinterpret_cast<uintptr_t*>(vtable + idx * sizeof(uintptr_t));
}

MemoryManager& mem() {
    static MemoryManager inst;
    return inst;
}

}
