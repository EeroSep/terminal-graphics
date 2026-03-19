#pragma once
#include "Vec4.h"

class Vec3 {
public:
    Vec3();
    Vec3(float x, float y, float z);
    Vec3(const Vec4& vec4);
    const float* getData() const;
    float x, y, z;
};