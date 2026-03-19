#include "Mat4.h"
#include "Vec4.h"
#include <cstring>

Mat4::Mat4() {
    data[0] = 1.0f; data[1] = 0.0f; data[2] = 0.0f; data[3] = 0.0f;
    data[4] = 0.0f; data[5] = 1.0f; data[6] = 0.0f; data[7] = 0.0f;
    data[8] = 0.0f; data[9] = 0.0f; data[10] = 1.0f; data[11] = 0.0f;
    data[12] = 0.0f; data[13] = 0.0f; data[14] = 0.0f; data[15] = 1.0f;
}

Mat4::Mat4(const float m[16]){
    std::memcpy(data, m, sizeof(float) * 16);
}

const float* Mat4::getData() const{
    return data;
}

Mat4 Mat4::operator*(const Mat4& other) const {
    float result[16];

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result[row * 4 + col] =
                data[row * 4 + 0] * other.data[0 * 4 + col] +
                data[row * 4 + 1] * other.data[1 * 4 + col] +
                data[row * 4 + 2] * other.data[2 * 4 + col] +
                data[row * 4 + 3] * other.data[3 * 4 + col];
        }
    }

    return Mat4(result);
}

Vec4 Mat4::operator*(const Vec4& vec) const {
    return Vec4(
        data[0] * vec.x + data[1] * vec.y + data[2] * vec.z + data[3] * vec.w,
        data[4] * vec.x + data[5] * vec.y + data[6] * vec.z + data[7] * vec.w,
        data[8] * vec.x + data[9] * vec.y + data[10] * vec.z + data[11] * vec.w,
        data[12] * vec.x + data[13] * vec.y + data[14] * vec.z + data[15] * vec.w
    );
}