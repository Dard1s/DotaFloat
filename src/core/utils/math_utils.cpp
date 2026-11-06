#include "math_utils.hpp"

namespace horizon::utils {

float calc_travel_time(const sdk::Vector3& from, const sdk::Vector3& to, float speed, float cast_point) {
    float dist = (to - from).length();
    if (speed <= 0.f) return cast_point;
    return cast_point + dist / speed;
}

float get_turn_time(const sdk::Vector3& from, const sdk::Vector3& dir, float turn_rate) {
    float len = dir.length2d();
    if (len < 0.01f || turn_rate <= 0.f) return 0.f;
    return (std::atan2(dir.y, dir.x) * kRad2Deg) / turn_rate;
}

bool is_in_range(const sdk::Vector3& a, const sdk::Vector3& b, float range) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return (dx*dx + dy*dy) <= range*range;
}

sdk::Vector3 predict_position(const sdk::Vector3& origin, const sdk::Vector3& velocity, float time) {
    return origin + velocity * time;
}

sdk::Vector3 extend_vector(const sdk::Vector3& from, const sdk::Vector3& to, float dist) {
    auto dir = (to - from).normalized();
    return from + dir * dist;
}

float distance_segment_to_point(const sdk::Vector3& a, const sdk::Vector3& b, const sdk::Vector3& p) {
    auto ab = b - a;
    auto ap = p - a;
    float t = ap.dot(ab) / ab.dot(ab);
    t = std::clamp(t, 0.f, 1.f);
    auto proj = a + ab * t;
    return (p - proj).length();
}

}
