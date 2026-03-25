#pragma once
class Vec4;

class Vec3 {
public:
    Vec3();
    Vec3(float x, float y, float z);
    Vec3(const Vec4& vec4);
    float x, y, z;
};