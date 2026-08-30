#include "Texture.h"
#include "../external/stb_image.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Texture::Texture(std::vector<Vec3> pixels, int width, int height)
    : pixels(std::move(pixels)), width(width), height(height) {}

std::shared_ptr<Texture> Texture::loadFromFile(const std::string& filepath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 3);
    if (!data) {
        std::cerr << "Texture::loadFromFile: failed to load " << filepath << std::endl;
        return nullptr;
    }

    std::vector<Vec3> pixels(static_cast<size_t>(width) * height);
    for (size_t i = 0; i < pixels.size(); i++) {
        pixels[i] = Vec3(
            data[i * 3 + 0] / 255.0f,
            data[i * 3 + 1] / 255.0f,
            data[i * 3 + 2] / 255.0f);
    }
    stbi_image_free(data);

    return std::shared_ptr<Texture>(new Texture(std::move(pixels), width, height));
}

Vec3 Texture::sample(float u, float v) const {
    u = u - std::floor(u);
    v = v - std::floor(v);

    int x = std::min(width - 1, static_cast<int>(u * width));
    int y = std::min(height - 1, static_cast<int>((1.0f - v) * height));

    return pixels[static_cast<size_t>(y) * width + x];
}
