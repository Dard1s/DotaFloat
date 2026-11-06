#pragma once
#include "../sdk/vector.hpp"
#include <cmath>

namespace horizon::utils {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kRad2Deg = 57.295779513082f;
constexpr float kDeg2Rad = 0.017453292519943295f;

float calc_travel_time(const sdk::Vector3& from, const sdk::Vector3& to, float speed, float cast_point = 0.f);
float get_turn_time(const sdk::Vector3& from, const sdk::Vector3& dir, float turn_rate);
bool is_in_range(const sdk::Vector3& a, const sdk::Vector3& b, float range);
sdk::Vector3 predict_position(const sdk::Vector3& origin, const sdk::Vector3& velocity, float time);
sdk::Vector3 extend_vector(const sdk::Vector3& from, const sdk::Vector3& to, float dist);
float distance_segment_to_point(const sdk::Vector3& a, const sdk::Vector3& b, const sdk::Vector3& p);

}
