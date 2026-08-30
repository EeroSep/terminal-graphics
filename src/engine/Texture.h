#pragma once
#include "../math/Vec3.h"
#include <memory>
#include <string>
#include <vector>

class Texture {
public:
    // Returns nullptr if the file can't be found or decoded.
    static std::shared_ptr<Texture> loadFromFile(const std::string& filepath);

    Vec3 sample(float u, float v) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    Texture(std::vector<Vec3> pixels, int width, int height);
    std::vector<Vec3> pixels; // row-major, top-left origin, linear [0,1] RGB
    int width, height;
};
