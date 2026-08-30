#pragma once
#include "../math/Vec3.h"
#include "Texture.h"
#include <cstdint>
#include <memory>

struct Material {
    Vec3 albedo;
    float ambient;
    float diffuse;
    std::shared_ptr<Texture> texture; // nullptr => solid albedo color

    Material(Vec3 albedo, float ambient = 0.25f, float diffuse = 0.85f, std::shared_ptr<Texture> texture = nullptr)
        : albedo(albedo), ambient(ambient), diffuse(diffuse), texture(std::move(texture)) {}

    Material(uint32_t rgba, float ambient = 0.25f, float diffuse = 0.85f);
};
