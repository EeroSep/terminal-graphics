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

void Rasterizer::rasterize_triangle(Framebuffer& fb, const Vec4& v0, const Vec4& v1, const Vec4& v2, uint32_t color) {
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
                fb.test_and_set(x, y, depth, color);
            }
        }
    }
}

void Rasterizer::draw(Framebuffer& fb, Object& obj, const Camera& camera) {
    std::vector<Vec4> vertices = obj.getVertices();
    std::vector<Face> faces = obj.getFaces();
    Mat4 model = obj.getModelMatrix();
    Mat4 vm = camera.getViewMatrix();
    Mat4 pm = camera.getProjectionMatrix();
    Mat4 vp = viewport(fb.getWidth(), fb.getHeight());
    for (Face& face : faces) {
        // Get vertices and transform to clip space
        Vec4 v0 = vertices[face.indices.x];
        Vec4 v1 = vertices[face.indices.y];
        Vec4 v2 = vertices[face.indices.z];
        v0 = pm * vm * model * v0;
        v1 = pm * vm * model * v1;
        v2 = pm * vm * model * v2;
        // Clip against near plane and rasterize
        std::vector<ClippedTriangle> tris = clipNearPlane(v0, v1, v2, face.color);
        if (tris.empty()) continue; 
        for (ClippedTriangle& tri : tris) {
            tri.v[0].ndc();
            tri.v[1].ndc();
            tri.v[2].ndc();
            tri.v[0] = vp * tri.v[0];
            tri.v[1] = vp * tri.v[1];
            tri.v[2] = vp * tri.v[2];
            rasterize_triangle(fb, tri.v[0], tri.v[1], tri.v[2], tri.color);
        }
    }
}

std::vector<ClippedTriangle> Rasterizer::clipNearPlane(const Vec4& v0, const Vec4& v1, const Vec4& v2, uint32_t color) {
    float wNear = 0.1f;
    std::vector<Vec4> inside, outside;
    Vec4 verts[3] = {v0, v1, v2};

    for (int i = 0; i < 3; i++) {
        if (verts[i].w >= wNear) inside.push_back(verts[i]);
        else outside.push_back(verts[i]);
    }

    if (inside.size() == 0) return {}; 
    
    // Pass the color into the ClippedTriangle constructor
    if (inside.size() == 3) {
        return {ClippedTriangle{ {v0, v1, v2}, color }}; 
    }

    if (inside.size() == 1) {
        Vec4 v1_n = intersect(inside[0], outside[0], wNear);
        Vec4 v2_n = intersect(inside[0], outside[1], wNear);
        return {ClippedTriangle{ {inside[0], v1_n, v2_n}, color }};
    } 

    if (inside.size() == 2) {
        Vec4 v2_n = intersect(inside[0], outside[0], wNear);
        Vec4 v3_n = intersect(inside[1], outside[0], wNear);
        return {
            ClippedTriangle{ {inside[0], inside[1], v2_n}, color },
            ClippedTriangle{ {inside[1], v3_n, v2_n}, color }
        };
    }
    return {};
}

Vec4 Rasterizer::intersect(const Vec4& v1, const Vec4& v2, float wNear) {
    float t = (wNear - v1.w) / (v2.w - v1.w);
    return Vec4(
        v1.x + t * (v2.x - v1.x),
        v1.y + t * (v2.y - v1.y),
        v1.z + t * (v2.z - v1.z),
        wNear
    );
}