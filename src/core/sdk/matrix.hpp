#pragma once
#include "vector.hpp"

namespace horizon::sdk {

struct Matrix3x4 {
    float m[3][4]{};
    Vector3 origin() const { return {m[0][3], m[1][3], m[2][3]}; }
};

struct Matrix4x4 {
    float m[4][4]{};
    Vector3 transform(const Vector3& v) const {
        return {
            v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3],
            v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3],
            v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3]
        };
    }
};

struct ViewMatrix {
    Matrix4x4 matrix;
    bool world_to_screen(const Vector3& world, Vector2& out, float screen_w, float screen_h) const;
};

}
