#include "Vec2.h"
#include <cmath>

float Vec2::dot(const Vec2& other) const {
        return x * other.x + y * other.y;
}

float Vec2::length() const {
    return sqrt(x * x + y * y);
}

float Vec2::angle(const Vec2& other) const {
    float dot = this->dot(other);
    return acos(dot / (other.length() * this->length()));
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}
