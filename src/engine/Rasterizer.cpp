#include "Rasterizer.h"
#include "../math/Vec2.h"
#include "Framebuffer.h"
#include <algorithm>

Rasterizer::Rasterizer() {

}

Rasterizer::~Rasterizer() {
    delete fb;
}

void Rasterizer::toBarycentric(const Vec4& v0, const Vec4& v1, const Vec4& v2, float x, float y) {
    
}

void Rasterizer::rasterize_triangle(const Vec4& v0, const Vec4& v1, const Vec4& v2) {
    // Bounding box
    int minx, miny, maxx, maxy;
    minx = std::max(std::min({v0.x, v1.x, v2.x}), 0.0f);
    miny = std::max(std::min({v0.y, v1.y, v2.y}), 0.0f);
    maxx = std::min(std::max({v0.x, v1.x, v2.x}), (float)(fb->getWidth()) );
    maxy = std::min(std::max({v0.y, v1.y, v2.y}), (float)(fb->getHeight()));

    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            float depth = 
            fb->test_and_set(x, y, depth, 0xFFFFFFFF);
        }
    }
}

void Rasterizer::draw(Framebuffer& fb, Object& obj) {
    // WIP
}