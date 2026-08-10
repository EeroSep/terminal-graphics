#pragma once
#include "../math/Vec3.h"
#include <cstdint>

struct Material {
    Vec3 albedo;
    float ambient;
    float diffuse;

    Material(Vec3 albedo, float ambient = 0.15f, float diffuse = 0.85f)
        : albedo(albedo), ambient(ambient), diffuse(diffuse) {}
        
    Material(uint32_t rgba, float ambient = 0.15f, float diffuse = 0.85f);
};
