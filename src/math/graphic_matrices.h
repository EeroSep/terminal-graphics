#include "Mat4.h"
#include "Vec4.h"
#include <cmath>

Mat4 translation(float tx, float ty, float tz) {
    float m[16] = {
        1.0f, 0.0f, 0.0f, tx,
        0.0f, 1.0f, 0.0f, ty,
        0.0f, 0.0f, 1.0f, tz,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Mat4(m);
}
Mat4 scaling(float sx, float sy, float sz) {
    float m[16] = {
        sx, 0.0f, 0.0f, 0.0f,
        0.0f, sy, 0.0f, 0.0f,
        0.0f, 0.0f, sz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Mat4(m);
}
Mat4 rotationX(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    float m[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, c,    -s,   0.0f,
        0.0f, s,    c,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Mat4(m);
}
Mat4 rotationY(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    float m[16] = {
        c,    0.0f, s,    0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -s,   0.0f, c,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Mat4(m);
}
Mat4 rotationZ(float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    float m[16] = {
        c,    -s,   0.0f, 0.0f,
        s,    c,    0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Mat4(m);
}

Mat4 rotationXYZ(float angleX, float angleY, float angleZ) {
    Mat4 rotX = rotationX(angleX);
    Mat4 rotY = rotationY(angleY);
    Mat4 rotZ = rotationZ(angleZ);

    return rotZ * rotY * rotX;
}

Mat4 perspective(float fov, float aspect, float near, float far) {
    float f = 1.0f / tanf(fov / 2.0f);
    float m[16] = {
        f / aspect, 0.0f, 0.0f,  0.0f, 
        0.0f,       f,    0.0f,  0.0f,
        0.0f,       0.0f, -(far + near) / (far - near), -(2 * far * near) / (far - near),
        0.0f,       0.0f, -1.0f, 0.0f
    };
    return Mat4(m);
}

Mat4 viewport(int w, int h) {
    float m[16] = {
        w / 2.0f, 0.0f,   0.0f, w / 2.0f,
        0.0f,   -h / 2.0f, 0.0f, h / 2.0f,
        0.0f,    0.0f,   0.5f, 0.5f,
        0.0f,    0.0f,   0.0f, 1.0f
    };
    return Mat4(m);
}