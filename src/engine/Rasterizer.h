#pragma once

#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include "Framebuffer.h"
#include "Object.h"
#include "Camera.h"

class Rasterizer {
public:
    Rasterizer();

    ~Rasterizer();
    void setFramebuffer(Framebuffer* framebuffer) { fb = framebuffer; }
    void draw(Framebuffer& fb, Object& obj, const Camera& camera);
private:
    unsigned int width, height; 
    Framebuffer* fb = nullptr;
    void rasterize_triangle(const Vec4&v0, const Vec4 &v1, const Vec4 &v2);
    void toBarycentric(const Vec4& v0, const Vec4& v1, const Vec4& v2, float x, float y);
};