#include "Mat4.h"
#include <cmath>
#pragma once
inline Mat4 translationM(float tx, float ty, float tz) {
    float m[16] = {
        1.0f, 0.0f, 0.0f, tx,
        0.0f, 1.0f, 0.0f, ty,
        0.0f, 0.0f, 1.0f, tz,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Mat4(m);
}
inline Mat4 scaling(float sx, float sy, float sz) {
    float m[16] = {
        sx, 0.0f, 0.0f, 0.0f,
        0.0f, sy, 0.0f, 0.0f,
        0.0f, 0.0f, sz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return Mat4(m);
}
inline Mat4 rotationX(float angle) {
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
inline Mat4 rotationY(float angle) {
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
inline Mat4 rotationZ(float angle) {
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

inline Mat4 rotationXYZ(float angleX, float angleY, float angleZ) {
    Mat4 rotX = rotationX(angleX);
    Mat4 rotY = rotationY(angleY);
    Mat4 rotZ = rotationZ(angleZ);

    return rotZ * rotY * rotX;
}

inline Mat4 viewport(int w, int h) {
    float m[16] = {
        w / 2.0f, 0.0f,   0.0f, w / 2.0f,
        0.0f,   -h / 2.0f, 0.0f, h / 2.0f,
        0.0f,    0.0f,   0.5f, 0.5f,
        0.0f,    0.0f,   0.0f, 1.0f
    };
    return Mat4(m);
}