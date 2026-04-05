#include "Framebuffer.h"

bool Framebuffer::test_and_set(int x, int y, float depth, unsigned int color) {
    int index = y * width + x;
    if (depth < depth_buffer[index]) {
        depth_buffer[index] = depth;
        color_buffer[index] = color;
        return true;
    }
    return false;
}
void Framebuffer::clear() {
    std::fill(color_buffer.begin(), color_buffer.end(), 0x000000FF);
    std::fill(depth_buffer.begin(), depth_buffer.end(), 1.0f);
}
