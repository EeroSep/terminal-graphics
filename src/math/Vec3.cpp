#include "Vec3.h"
#include "Vec4.h"
#include <cmath>

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
float Vec3::dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator+(const Vec3& other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator*(float scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::cross(const Vec3& other) const {
    return Vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

float Vec3::length() const {
    return sqrtf(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const {
    float len = length();
    if (len > 0) {
        return Vec3(x / len, y / len, z / len);
    }
    return Vec3(0, 0, 0); 
}