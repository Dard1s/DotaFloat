#pragma once
#include <cstdint>
#include <string>
#include <array>
#include <memory>
#include <vector>
#include <functional>

namespace horizon {

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using f32 = float;
using f64 = double;

constexpr u32 kBuildVersion = 0x024A1B;
constexpr const char* kProjectName = "DotaFloat";
constexpr const char* kLoaderMutex = "dfloat_mtx_9f3a21";

template<typename T, size_t N>
constexpr size_t array_size(T(&)[N]) { return N; }

}
