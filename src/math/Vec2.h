#pragma once

class Vec2 {
public:
    Vec2() : x(0), y(0) {};
    Vec2(float x, float y) : x(x), y(y) {};

    float dot(const Vec2& other) const;
    float length() const;
    float angle(const Vec2& other) const;
    
    float x, y;
};