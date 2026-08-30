#pragma once

#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include "Framebuffer.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"

struct ClippedTriangle {
    Vec4 v[3];
    Vec2 uv[3];
    uint32_t color;          // used when texture == nullptr
    const Texture* texture;  // nullptr => solid color
    float shade;             // flat lighting factor, applied to texture samples per-pixel
};

class Rasterizer {
public:
    Rasterizer();

    ~Rasterizer();
    void draw(Framebuffer& fb, Object& obj, const Camera& camera, const Light& light);
private:
    struct RasterVertex { Vec4 pos; Vec2 uv; };

    unsigned int width, height;
    std::vector<Vec4> viewSpaceCache; // reused across frames to avoid reallocating every draw() call
    void rasterize_triangle(Framebuffer& fb, const Vec4& v0, const Vec4& v1, const Vec4& v2,
                             float invW0, float invW1, float invW2,
                             const Vec2& uv0, const Vec2& uv1, const Vec2& uv2,
                             uint32_t flatColor, const Texture* texture, float shade);
    Vec3 toBarycentric(const Vec4& v0, const Vec4& v1, const Vec4& v2, float x, float y);
    std::vector<ClippedTriangle> clipNearPlane(const Vec4& v0, const Vec4& v1, const Vec4& v2,
                                                const Vec2& uv0, const Vec2& uv1, const Vec2& uv2,
                                                uint32_t color, const Texture* texture, float shade);
    RasterVertex intersect(const RasterVertex& v1, const RasterVertex& v2, float wNear);
};