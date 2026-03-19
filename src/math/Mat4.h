#pragma once
#include "Vec4.h"
class Mat4{
    public:
        Mat4();
        Mat4(const float m[16]);
        const float* getData() const;
        Mat4 operator*(const Mat4& other) const;
        Vec4 operator*(const Vec4& vec) const;
    private:
        float data[16];
};