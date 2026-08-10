#pragma once

#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include "Framebuffer.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"

struct ClippedTriangle {
    Vec4 v[3];
    uint32_t color;
};

class Rasterizer {
public:
    Rasterizer();

    ~Rasterizer();
    void draw(Framebuffer& fb, Object& obj, const Camera& camera, const Light& light);
private:
    unsigned int width, height;
    std::vector<Vec4> viewSpaceCache; // reused across frames to avoid reallocating every draw() call
    void rasterize_triangle(Framebuffer& fb, const Vec4&v0, const Vec4 &v1, const Vec4 &v2, uint32_t color);
    Vec3 toBarycentric(const Vec4& v0, const Vec4& v1, const Vec4& v2, float x, float y);
    std::vector<ClippedTriangle> clipNearPlane(const Vec4& v0, const Vec4& v1, const Vec4& v2, uint32_t color);
    Vec4 intersect(const Vec4& v1, const Vec4& v2, float wNear);
};