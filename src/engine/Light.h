#pragma once
#include "../math/Vec3.h"

struct Light {
    Vec3 direction; // points FROM a lit surface TOWARD the light, normalized
    float intensity;

    Light(Vec3 direction, float intensity = 1.0f)
        : direction(direction.normalize()), intensity(intensity) {}
};
