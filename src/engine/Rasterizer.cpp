#include "Rasterizer.h"
#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "Framebuffer.h"
#include <algorithm>
#include "../math/graphic_matrices.h"
#include "../utility/Colors.h"
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

void Rasterizer::rasterize_triangle(Framebuffer& fb, const Vec4& v0, const Vec4& v1, const Vec4& v2,
                                     float invW0, float invW1, float invW2,
                                     const Vec2& uv0, const Vec2& uv1, const Vec2& uv2,
                                     uint32_t flatColor, const Texture* texture, float shade) {
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

                uint32_t color = flatColor;
                if (texture) {
                    // Perspective-correct interpolation: UVs (and 1/w itself) are
                    // linear in screen space only after dividing by w, so interpolate
                    // uv/w and 1/w linearly, then recover uv by dividing back out.
                    float invW = weights.x * invW0 + weights.y * invW1 + weights.z * invW2;
                    float u = (weights.x * uv0.x * invW0 + weights.y * uv1.x * invW1 + weights.z * uv2.x * invW2) / invW;
                    float v = (weights.x * uv0.y * invW0 + weights.y * uv1.y * invW1 + weights.z * uv2.y * invW2) / invW;

                    Vec3 shaded = texture->sample(u, v) * shade;
                    color = Colors::rgba(
                        static_cast<uint8_t>(std::clamp(shaded.x, 0.0f, 1.0f) * 255.0f),
                        static_cast<uint8_t>(std::clamp(shaded.y, 0.0f, 1.0f) * 255.0f),
                        static_cast<uint8_t>(std::clamp(shaded.z, 0.0f, 1.0f) * 255.0f),
                        255);
                }

                fb.test_and_set(x, y, depth, color);
            }
        }
    }
}

void Rasterizer::draw(Framebuffer& fb, Object& obj, const Camera& camera, const Light& light) {
    const std::vector<Vec4>& vertices = obj.getVertices();
    const std::vector<Face>& faces = obj.getFaces();
    Mat4 model = obj.getModelMatrix();
    Mat4 vm = camera.getViewMatrix();
    Mat4 pm = camera.getProjectionMatrix();
    Mat4 mv = vm * model;
    Mat4 vp = viewport(fb.getWidth(), fb.getHeight());

    // Transform each unique vertex into view space once, rather than once per
    // face that references it.
    if (viewSpaceCache.size() < vertices.size()) viewSpaceCache.resize(vertices.size());
    for (size_t i = 0; i < vertices.size(); i++) viewSpaceCache[i] = mv * vertices[i];

    // The light direction is defined in world space; w = 0 drops the view
    // matrix's translation so only its rotation is applied.
    Vec4 lightDirView4 = vm * Vec4(light.direction, 0.0f);
    Vec3 lightDirView = Vec3(lightDirView4.x, lightDirView4.y, lightDirView4.z).normalize();

    for (const Face& face : faces) {
        // Backface cull in view space: the camera sits at the origin looking
        // down -Z, so a vertex's view-space position doubles as the direction
        // from the camera to that point on the face.
        const Vec4& view0 = viewSpaceCache[face.indices.x];
        const Vec4& view1 = viewSpaceCache[face.indices.y];
        const Vec4& view2 = viewSpaceCache[face.indices.z];
        Vec3 p0(view0.x, view0.y, view0.z);
        Vec3 p1(view1.x, view1.y, view1.z);
        Vec3 p2(view2.x, view2.y, view2.z);
        Vec3 normal = (p1 - p0).cross(p2 - p0);
        if (normal.dot(p0) >= 0.0f) continue;

        // Flat (per-face) Lambertian shading, reusing the normal above.
        Vec3 n = normal.normalize();
        float ndotl = std::max(0.0f, n.dot(lightDirView));
        float shade = std::min(1.0f, face.material.ambient + face.material.diffuse * ndotl * light.intensity);
        Vec3 shadedAlbedo = face.material.albedo * shade;
        uint32_t color = Colors::rgba(
            static_cast<uint8_t>(std::clamp(shadedAlbedo.x, 0.0f, 1.0f) * 255.0f),
            static_cast<uint8_t>(std::clamp(shadedAlbedo.y, 0.0f, 1.0f) * 255.0f),
            static_cast<uint8_t>(std::clamp(shadedAlbedo.z, 0.0f, 1.0f) * 255.0f),
            255);
        const Texture* texture = face.material.texture.get();

        // Transform to clip space
        Vec4 v0 = pm * view0;
        Vec4 v1 = pm * view1;
        Vec4 v2 = pm * view2;

        // Frustum cull: cheap broad-phase test that skips a triangle sitting
        // entirely outside one clip plane, before paying for near-plane clip
        // and rasterization. Near is handled separately below since a
        // triangle can straddle it and still be partially visible.
        if ((v0.x > v0.w && v1.x > v1.w && v2.x > v2.w) ||
            (v0.x < -v0.w && v1.x < -v1.w && v2.x < -v2.w) ||
            (v0.y > v0.w && v1.y > v1.w && v2.y > v2.w) ||
            (v0.y < -v0.w && v1.y < -v1.w && v2.y < -v2.w) ||
            (v0.z > v0.w && v1.z > v1.w && v2.z > v2.w)) {
            continue;
        }

        // Clip against near plane and rasterize
        std::vector<ClippedTriangle> tris = clipNearPlane(v0, v1, v2, face.uv[0], face.uv[1], face.uv[2], color, texture, shade);
        if (tris.empty()) continue;
        for (ClippedTriangle& tri : tris) {
            // w before ndc() is the clip-space w (== view-space -z); its reciprocal
            // is what perspective-correct UV interpolation needs, so it must be
            // captured before ndc() overwrites w with 1.
            float invW0 = 1.0f / tri.v[0].w;
            float invW1 = 1.0f / tri.v[1].w;
            float invW2 = 1.0f / tri.v[2].w;
            tri.v[0].ndc();
            tri.v[1].ndc();
            tri.v[2].ndc();
            tri.v[0] = vp * tri.v[0];
            tri.v[1] = vp * tri.v[1];
            tri.v[2] = vp * tri.v[2];
            rasterize_triangle(fb, tri.v[0], tri.v[1], tri.v[2], invW0, invW1, invW2,
                                tri.uv[0], tri.uv[1], tri.uv[2], tri.color, tri.texture, tri.shade);
        }
    }
}

std::vector<ClippedTriangle> Rasterizer::clipNearPlane(const Vec4& v0, const Vec4& v1, const Vec4& v2,
                                                         const Vec2& uv0, const Vec2& uv1, const Vec2& uv2,
                                                         uint32_t color, const Texture* texture, float shade) {
    float wNear = 0.1f;
    std::vector<RasterVertex> inside, outside;
    RasterVertex verts[3] = {{v0, uv0}, {v1, uv1}, {v2, uv2}};

    for (int i = 0; i < 3; i++) {
        if (verts[i].pos.w >= wNear) inside.push_back(verts[i]);
        else outside.push_back(verts[i]);
    }

    if (inside.size() == 0) return {};

    auto makeTri = [&](const RasterVertex& a, const RasterVertex& b, const RasterVertex& c) {
        return ClippedTriangle{ {a.pos, b.pos, c.pos}, {a.uv, b.uv, c.uv}, color, texture, shade };
    };

    if (inside.size() == 3) {
        return {makeTri(verts[0], verts[1], verts[2])};
    }

    if (inside.size() == 1) {
        RasterVertex v1_n = intersect(inside[0], outside[0], wNear);
        RasterVertex v2_n = intersect(inside[0], outside[1], wNear);
        return {makeTri(inside[0], v1_n, v2_n)};
    }

    if (inside.size() == 2) {
        RasterVertex v2_n = intersect(inside[0], outside[0], wNear);
        RasterVertex v3_n = intersect(inside[1], outside[0], wNear);
        return {
            makeTri(inside[0], inside[1], v2_n),
            makeTri(inside[1], v3_n, v2_n)
        };
    }
    return {};
}

Rasterizer::RasterVertex Rasterizer::intersect(const RasterVertex& v1, const RasterVertex& v2, float wNear) {
    float t = (wNear - v1.pos.w) / (v2.pos.w - v1.pos.w);
    return RasterVertex{
        Vec4(
            v1.pos.x + t * (v2.pos.x - v1.pos.x),
            v1.pos.y + t * (v2.pos.y - v1.pos.y),
            v1.pos.z + t * (v2.pos.z - v1.pos.z),
            wNear
        ),
        v1.uv + (v2.uv - v1.uv) * t
    };
}