#pragma once

#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"

struct Framebuffer {
    Framebuffer(int width, int height) : width(width), height(height) {}
    int width, height;
}; 

class Rasterizer {
public:
    Rasterizer(int width, int height);

    ~Rasterizer();

    void rasterize_triangle(const Vec2&v0, const Vec2 &v1, const Vec2 &v2);

    inline Framebuffer* getFramebuffer() const { return fb; }
private:
    Framebuffer* fb;
    void init_framebuffer(int width, int height);
};