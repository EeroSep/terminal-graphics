#include "Rasterizer.h"
#include "../math/Vec2.h"
#include "Framebuffer.h"
#include <algorithm>
void Rasterizer::init_framebuffer(int width, int height) {
    fb = new Framebuffer(width, height);
}

Rasterizer::Rasterizer() {

}

Rasterizer::~Rasterizer() {
    delete fb;
}

void Rasterizer::rasterize_triangle(const Vec2& v0, const Vec2& v1, const Vec2& v2) {
    int minx, miny, maxx, maxy;
    minx = std::max(std::min({v0.x, v1.x, v2.x}), 0.0f);
    miny = std::max(std::min({v0.y, v1.y, v2.y}), 0.0f);
    maxx = std::min(std::max({v0.x, v1.x, v2.x}), (float)(fb->getWidth()) );
    maxy = std::min(std::max({v0.y, v1.y, v2.y}), (float)(fb->getHeight()));

    // WIP, decided to work on framebuffer first
}

void Rasterizer::draw(Framebuffer& fb, const Camera& camera, Object& obj) {
    // WIP
}

void Rasterizer::viewMatrix(const Vec3& eye, const Vec3& center, const Vec3& up) {
    // WIP
}

void Rasterizer::projectionMatrix(float fov, float aspect, float near, float far) {
    // WIP
}
