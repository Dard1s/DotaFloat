#pragma once
#include <windows.h>
#include <string>
#include <vector>

namespace horizon::loader::injector {

struct ManualMapResult {
    bool success;
    uintptr_t remote_base;
    std::string error;
};

class ManualMapper {
public:
    ManualMapResult map_image(DWORD pid, const std::string& dll_path);
    ManualMapResult map_buffer(DWORD pid, const std::vector<uint8_t>& image);
private:
    bool fix_imports(uintptr_t local_base, uintptr_t remote_base, DWORD pid);
    bool fix_relocations(uintptr_t local_base, uintptr_t remote_base, int64_t delta);
    uintptr_t allocate_remote(DWORD pid, size_t size);
    bool write_remote(DWORD pid, uintptr_t remote, const void* local, size_t size);
    bool call_dllmain(DWORD pid, uintptr_t remote_base);
    uintptr_t get_loader_entry(uintptr_t remote_base) const;
};

}
