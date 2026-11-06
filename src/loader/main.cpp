#include "injector/manual_map.hpp"
#include "injector/pe_utils.hpp"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <thread>

DWORD find_process(const char* name) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;
    PROCESSENTRY32 pe{sizeof(pe)};
    DWORD pid = 0;
    if (Process32First(snap, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, name) == 0) { pid = pe.th32ProcessID; break; }
        } while (Process32Next(snap, &pe));
    }
    CloseHandle(snap);
    return pid;
}

bool enable_privilege() {
    HANDLE token{};
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) return false;
    LUID luid{};
    LookupPrivilegeValueA(nullptr, SE_DEBUG_NAME, &luid);
    TOKEN_PRIVILEGES tp{};
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr);
    CloseHandle(token);
    return true;
}

int main(int argc, char* argv[]) {
    SetConsoleTitleA("DotaFloat Loader v2.4.11 - dota2 internal");

    std::cout << "  ____        _        ______ _            _   \n";
    std::cout << " |  _ \\  ___ | |_ __ _|  ___| | ___  __ _| |_ \n";
    std::cout << " | | | |/ _ \\| __/ _` | |_  | |/ _ \\/ _` | __|\n";
    std::cout << " | |_| | (_) | || (_| |  _| | | (_) | (_| | |_ \n";
    std::cout << " |____/ \\___/ \\__\\__,_|_|   |_|\\___/ \\__,_|\\__|\n";
    std::cout << "                DotaFloat v2.4.11               \n";
    std::cout << "                                                \n";

    enable_privilege();

    std::cout << "[*] Waiting for dota2.exe...\n";
    DWORD pid = 0;
    while (!(pid = find_process("dota2.exe"))) {
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
    std::cout << "[+] Found dota2.exe pid=" << pid << "\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    horizon::loader::injector::ManualMapper mapper;
    std::string dll_path = "dotafloat.core.dll";
    if (argc > 1) dll_path = argv[1];

    std::cout << "[*] Mapping " << dll_path << "...\n";
    auto result = mapper.map_image(pid, dll_path);
    if (!result.success) {
        std::cout << "[!] Injection failed: " << result.error << "\n";
        std::cout << "[!] Try restarting Steam as admin.\n";
        system("pause");
        return 1;
    }

    std::cout << "[+] Injected at 0x" << std::hex << result.remote_base << std::dec << "\n";
    std::cout << "[+] DotaFloat loaded. Press END in-game to unload.\n";
    std::cout << "[+] Loader will exit in 5s.\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
