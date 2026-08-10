#include "Material.h"

Material::Material(uint32_t rgba, float ambient, float diffuse)
    : albedo(
          ((rgba >> 0) & 0xFF) / 255.0f,
          ((rgba >> 8) & 0xFF) / 255.0f,
          ((rgba >> 16) & 0xFF) / 255.0f),
      ambient(ambient), diffuse(diffuse) {}
