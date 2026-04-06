#pragma once

#include "../math/Vec2.h"
#include <vector>

class Framebuffer {
    public:
    Framebuffer(unsigned int width, unsigned int height) : width(width), height(height) {}; 
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }
    void resize(unsigned int new_width, unsigned int new_height);
    void clear(); 
    bool test_and_set(int x, int y, float depth, unsigned int color); 
    const unsigned int* getColorBuffer() const { return color_buffer.data(); }
    
    private:
    unsigned int width, height;
    std::vector<unsigned int> color_buffer;
    std::vector<float> depth_buffer;
};