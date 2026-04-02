#pragma once

#include "../math/Vec2.h"
#include <vector>

class Framebuffer {
    public:
    Framebuffer(int width, int height) : width(width), height(height) {}; 
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void clear(); 
    bool test_and_set(int x, int y, float depth, unsigned int color); 

    private:
    int width, height;
    std::vector<unsigned int> color_buffer;
    std::vector<float> depth_buffer;
};