#pragma once
#include <windows.h>
#include <vector>
#include <string>

namespace horizon::loader::injector {

bool is_valid_pe(const std::vector<uint8_t>& data);
size_t get_image_size(const std::vector<uint8_t>& data);
uintptr_t get_entry_point_rva(const std::vector<uint8_t>& data);
std::vector<std::string> get_imports(const std::vector<uint8_t>& data);
bool has_tls_callback(const std::vector<uint8_t>& data);
DWORD get_checksum(const std::vector<uint8_t>& data);

}
