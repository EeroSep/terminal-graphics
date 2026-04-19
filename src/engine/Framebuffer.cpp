#include "Framebuffer.h"
#include <cstdint>

Framebuffer::Framebuffer(unsigned int width, unsigned int height) : width(width), height(height) {
    resize(width, height);
}

bool Framebuffer::test_and_set(int x, int y, float depth, uint32_t color) {
    if (x < 0 || x >= (int)width || y < 0 || y >= (int)height) return false;
    int index = y * width + x;
    if (depth < depth_buffer[index]) {
        depth_buffer[index] = depth;
        color_buffer[index] = color;
        return true;
    }
    return false;
}

void Framebuffer::resize(unsigned int new_width, unsigned int new_height) {
    width = new_width;
    height = new_height;
    color_buffer.assign(width * height, 0x000000FF); 
    depth_buffer.assign(width * height, 1.0f); 
}

void Framebuffer::clear() {
    std::fill(color_buffer.begin(), color_buffer.end(), 0x000000FF);
    std::fill(depth_buffer.begin(), depth_buffer.end(), 1.0f);
}
