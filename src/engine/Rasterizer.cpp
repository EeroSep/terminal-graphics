#include "Rasterizer.h"
#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "Framebuffer.h"
#include <algorithm>
#include "../math/graphic_matrices.h"
#include <iostream>

Rasterizer::Rasterizer() {

}

Rasterizer::~Rasterizer() {
    
}

Vec3 Rasterizer::toBarycentric(const Vec4& v0, const Vec4& v1, const Vec4& v2, float x, float y) {
    float det = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);

    if (std::abs(det) < 0.00001f) return Vec3(-1, -1, -1); 

    float w0 = ((v1.y - v2.y) * (x - v2.x) + (v2.x - v1.x) * (y - v2.y)) / det;
    float w1 = ((v2.y - v0.y) * (x - v2.x) + (v0.x - v2.x) * (y - v2.y)) / det;
    float w2 = 1.0f - w0 - w1;

    return Vec3(w0, w1, w2);   
}

void Rasterizer::rasterize_triangle(Framebuffer& fb, const Vec4& v0, const Vec4& v1, const Vec4& v2) {
    // Bounding box
    int minx = std::max(0, (int)std::floor(std::min({v0.x, v1.x, v2.x})));
    int miny = std::max(0, (int)std::floor(std::min({v0.y, v1.y, v2.y})));
    int maxx = std::min((int)fb.getWidth() - 1, (int)std::ceil(std::max({v0.x, v1.x, v2.x})));
    int maxy = std::min((int)fb.getHeight() - 1, (int)std::ceil(std::max({v0.y, v1.y, v2.y})));

    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            Vec3 weights = toBarycentric(v0, v1, v2, (float)x + 0.5f, (float)y + 0.5f);
            if (weights.x >= 0 && weights.y >= 0 && weights.z >= 0) {
                float depth = weights.x * v0.z + weights.y * v1.z + weights.z * v2.z;
                fb.test_and_set(x, y, depth, 0xFFFFFFFF);
            }
        }
    }
}

void Rasterizer::draw(Framebuffer& fb, Object& obj, const Camera& camera) {
    std::vector<Vec4> vertices = obj.getVertices();
    std::vector<Vec3> faces = obj.getFaces();
    Mat4 model = obj.getModelMatrix();
    Mat4 vm = camera.getViewMatrix();
    Mat4 pm = camera.getProjectionMatrix();
    Mat4 vp = viewport(fb.getWidth(), fb.getHeight());
    for (const Vec3& face : faces) {
        Vec4 v0 = vertices[face.x];
        Vec4 v1 = vertices[face.y];
        Vec4 v2 = vertices[face.z];
        v0 = pm * vm * model * v0;
        v1 = pm * vm * model * v1;
        v2 = pm * vm * model * v2;
        v0.ndc();
        v1.ndc();
        v2.ndc();
        v0 = vp * v0;
        v1 = vp * v1;
        v2 = vp * v2;
        rasterize_triangle(fb, v0, v1, v2);
    }
}