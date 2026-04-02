#pragma once

#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include "Framebuffer.h"


class Rasterizer {
public:
    Rasterizer(int width, int height);

    ~Rasterizer();

    void rasterize_triangle(const Vec2&v0, const Vec2 &v1, const Vec2 &v2);

private:
    Framebuffer* fb;
    void init_framebuffer(int width, int height);
};