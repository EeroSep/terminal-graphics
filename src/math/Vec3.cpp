#include "Vec3.h"
#include "Vec4.h"
#include <cassert>

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
Vec3::Vec3(const Vec4& vec4) {
    if (vec4.w != 0) {
        x = vec4.x / vec4.w;
        y = vec4.y / vec4.w;
        z = vec4.z / vec4.w;
    } else {
        x = vec4.x;
        y = vec4.y;
        z = vec4.z;
    }
}