#include "Vec4.h"
#include "Vec3.h"

Vec4::Vec4() : x(0), y(0), z(0), w(0) {}
Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Vec4::Vec4(const Vec3& vec3, float w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) {}
const float* Vec4::getData() const {
    return &x;
}
