#pragma once
#include <cmath>
#include <algorithm>

namespace horizon::sdk {

struct Vector2 {
    float x{}, y{};
    constexpr Vector2() = default;
    constexpr Vector2(float _x, float _y) : x(_x), y(_y) {}
    Vector2 operator+(const Vector2& o) const { return {x+o.x, y+o.y}; }
    Vector2 operator-(const Vector2& o) const { return {x-o.x, y-o.y}; }
    Vector2 operator*(float s) const { return {x*s, y*s}; }
    float length() const { return std::sqrt(x*x + y*y); }
    float length2d() const { return std::sqrt(x*x + y*y); }
    float dist_to(const Vector2& o) const { return (*this - o).length(); }
};

struct Vector3 {
    float x{}, y{}, z{};
    constexpr Vector3() = default;
    constexpr Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3 operator+(const Vector3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vector3 operator-(const Vector3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vector3 operator*(float s) const { return {x*s, y*s, z*s}; }
    Vector3 operator/(float s) const { return {x/s, y/s, z/s}; }
    float dot(const Vector3& o) const { return x*o.x + y*o.y + z*o.z; }
    float length() const { return std::sqrt(x*x + y*y + z*z); }
    float length2d() const { return std::sqrt(x*x + y*y); }
    Vector3 normalized() const { float l = length(); return l > 0.f ? *this / l : Vector3{}; }
};

struct QAngle {
    float pitch{}, yaw{}, roll{};
};

}
