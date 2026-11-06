#include "manual_map.hpp"
#include "pe_utils.hpp"
#include <psapi.h>
#include <tlhelp32.h>

namespace horizon::loader::injector {

ManualMapResult ManualMapper::map_image(DWORD pid, const std::string& dll_path) {
    HANDLE hFile = CreateFileA(dll_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) return {false, 0, "open failed"};
    DWORD size = GetFileSize(hFile, nullptr);
    std::vector<uint8_t> buf(size);
    DWORD read{};
    ReadFile(hFile, buf.data(), size, &read, nullptr);
    CloseHandle(hFile);
    return map_buffer(pid, buf);
}

ManualMapResult ManualMapper::map_buffer(DWORD pid, const std::vector<uint8_t>& image) {
    if (image.size() < sizeof(IMAGE_DOS_HEADER)) return {false, 0, "invalid image"};
    auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(image.data());
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return {false, 0, "dos magic"};
    auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(image.data() + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return {false, 0, "nt magic"};

    size_t image_size = nt->OptionalHeader.SizeOfImage;
    uintptr_t remote = allocate_remote(pid, image_size);
    if (!remote) return {false, 0, "alloc failed"};

    if (!write_remote(pid, remote, image.data(), nt->OptionalHeader.SizeOfHeaders)) {
        return {false, 0, "write headers failed"};
    }

    auto* sec = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; ++i) {
        if (sec[i].SizeOfRawData == 0) continue;
        uintptr_t dst = remote + sec[i].VirtualAddress;
        const void* src = image.data() + sec[i].PointerToRawData;
        if (!write_remote(pid, dst, src, sec[i].SizeOfRawData)) return {false, 0, "section write failed"};
    }

    int64_t delta = static_cast<int64_t>(remote) - static_cast<int64_t>(nt->OptionalHeader.ImageBase);
    uintptr_t local_copy = reinterpret_cast<uintptr_t>(image.data());
    fix_relocations(local_copy, remote, delta);
    fix_imports(local_copy, remote, pid);
    call_dllmain(pid, remote);
    return {true, remote, ""};
}

bool ManualMapper::fix_imports(uintptr_t local_base, uintptr_t remote_base, DWORD pid) {
    (void)local_base;(void)remote_base;(void)pid;
    return true;
}

bool ManualMapper::fix_relocations(uintptr_t local_base, uintptr_t remote_base, int64_t delta) {
    (void)local_base;(void)remote_base;(void)delta;
    return true;
}

uintptr_t ManualMapper::allocate_remote(DWORD pid, size_t size) {
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) return 0;
    uintptr_t addr = reinterpret_cast<uintptr_t>(VirtualAllocEx(hProc, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READ));
    CloseHandle(hProc);
    return addr;
}

bool ManualMapper::write_remote(DWORD pid, uintptr_t remote, const void* local, size_t size) {
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) return false;
    SIZE_T written{};
    bool ok = WriteProcessMemory(hProc, reinterpret_cast<void*>(remote), local, size, &written) != 0;
    CloseHandle(hProc);
    return ok && written == size;
}

bool ManualMapper::call_dllmain(DWORD pid, uintptr_t remote_base) {
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) return false;
    uintptr_t entry = get_loader_entry(remote_base);
    HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry), reinterpret_cast<void*>(remote_base), 0, nullptr);
    if (!hThread) { CloseHandle(hProc); return false; }
    WaitForSingleObject(hThread, 2000);
    CloseHandle(hThread);
    CloseHandle(hProc);
    return true;
}

uintptr_t ManualMapper::get_loader_entry(uintptr_t remote_base) const {
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(remote_base);
    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(remote_base + dos->e_lfanew);
    return remote_base + nt->OptionalHeader.AddressOfEntryPoint;
}

}
