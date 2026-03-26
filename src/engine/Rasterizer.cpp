#include "Rasterizer.h"
#include "../math/Vec2.h"
Rasterizer::init_framebuffer(int width, int height) {
    fb = new Framebuffer(width, height);
}

Rasterizer::Rasterizer(int width, int height) {
    init_framebuffer(width, height);
}

Rasterizer::~Rasterizer() {
    delete fb;
}
