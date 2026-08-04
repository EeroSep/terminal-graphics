#pragma once

#include "../math/Vec2.h"
#include <array>
#include <vector>
#include <cstdint>

class Framebuffer {
    public:
    Framebuffer(unsigned int width, unsigned int height);
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }
    void resize(unsigned int new_width, unsigned int new_height);
    void clear();
    bool test_and_set(int x, int y, float depth, uint32_t color);
    const uint32_t* getColorBuffer() const { return color_buffer[frontIndex()].data(); }
    void swapBuffer() { backIndex = frontIndex(); }

    private:
    unsigned int frontIndex() const { return 1 - backIndex; }
    unsigned int width, height;
    unsigned int backIndex = 0;
    std::array<std::vector<uint32_t>, 2> color_buffer;
    std::vector<float> depth_buffer;
};