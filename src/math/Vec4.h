#pragma once
class Vec3;

class Vec4{
public:
    Vec4();
    Vec4(float x, float y, float z, float w);
    Vec4(const Vec3& vec3, float w);
    const float* getData() const;
    float x, y, z, w;
};