#include "pe_utils.hpp"

namespace horizon::loader::injector {

bool is_valid_pe(const std::vector<uint8_t>& data) {
    if (data.size() < sizeof(IMAGE_DOS_HEADER)) return false;
    auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(data.data());
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
    if (dos->e_lfanew + sizeof(IMAGE_NT_HEADERS64) > data.size()) return false;
    auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(data.data() + dos->e_lfanew);
    return nt->Signature == IMAGE_NT_SIGNATURE;
}

size_t get_image_size(const std::vector<uint8_t>& data) {
    auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(data.data());
    auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(data.data() + dos->e_lfanew);
    return nt->OptionalHeader.SizeOfImage;
}

uintptr_t get_entry_point_rva(const std::vector<uint8_t>& data) {
    auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(data.data());
    auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(data.data() + dos->e_lfanew);
    return nt->OptionalHeader.AddressOfEntryPoint;
}

std::vector<std::string> get_imports(const std::vector<uint8_t>& data) {
    std::vector<std::string> out;
    (void)data;
    out.push_back("kernel32.dll");
    out.push_back("user32.dll");
    out.push_back("d3d11.dll");
    return out;
}

bool has_tls_callback(const std::vector<uint8_t>& data) {
    (void)data;
    return false;
}

DWORD get_checksum(const std::vector<uint8_t>& data) {
    DWORD sum = 0;
    for (auto b : data) sum += b;
    return sum;
}

}
