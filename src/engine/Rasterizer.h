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
#include <thread>
#include <mutex>
#include <condition_variable>

struct ClippedTriangle {
    Vec4 v[3];
    Vec2 uv[3];
    uint32_t color;          // used when texture == nullptr
    const Texture* texture;  // nullptr => solid color
    float shade;             // flat lighting factor, applied to texture samples per-pixel
};

// A fully projected, screen-space triangle ready for rasterization: produced
// by draw() and consumed (possibly by another thread) by flush().
struct ScreenTriangle {
    Vec4 v[3];       // viewport-space x/y, NDC depth in z
    float invW[3];   // 1/clip-space w, for perspective-correct UV interpolation
    Vec2 uv[3];
    uint32_t color;
    const Texture* texture;
    float shade;
};

class Rasterizer {
public:
    Rasterizer();
    ~Rasterizer();

    // Transforms one object's faces (cull, shade, clip, project) and queues
    // the resulting screen-space triangles for the next flush() call. Does
    // not touch the framebuffer's pixel data.
    void draw(Framebuffer& fb, Object& obj, const Camera& camera, const Light& light);

    // Rasterizes every triangle queued by draw() since the last flush(),
    // splitting the framebuffer's rows round-robin across a persistent
    // thread pool (row y goes to thread y % numBands). Each thread only
    // ever writes pixels in its own row set, so no locking is needed on the
    // color/depth buffers. Interleaving rows (instead of giving each thread
    // one contiguous slice) keeps threads balanced even when triangle
    // density is very uneven across screen height, e.g. sky vs. ground.
    void flush(Framebuffer& fb);

private:
    struct RasterVertex { Vec4 pos; Vec2 uv; };

    std::vector<Vec4> viewSpaceCache; // reused across frames to avoid reallocating every draw() call
    std::vector<ScreenTriangle> triangleBuffer; // accumulated by draw(), consumed by flush()

    void rasterize_triangle(Framebuffer& fb, const ScreenTriangle& tri, unsigned int bandIndex, unsigned int numBands);
    Vec3 toBarycentric(const Vec4& v0, const Vec4& v1, const Vec4& v2, float x, float y);
    std::vector<ClippedTriangle> clipNearPlane(const Vec4& v0, const Vec4& v1, const Vec4& v2,
                                                const Vec2& uv0, const Vec2& uv1, const Vec2& uv2,
                                                uint32_t color, const Texture* texture, float shade);
    RasterVertex intersect(const RasterVertex& v1, const RasterVertex& v2, float wNear);

    // Persistent thread pool: workers sleep between frames and wake on an
    // epoch bump instead of being spawned/joined every frame. Band 0
    // (rows 0, numBands, 2*numBands, ...) is rasterized inline by whichever
    // thread calls flush() so no core sits idle waiting on the others.
    unsigned int numBands;
    std::vector<std::thread> workers;
    std::mutex poolMtx;
    std::condition_variable startCv, doneCv;
    bool stop = false;
    unsigned int epoch = 0;
    unsigned int workersDone = 0;
    Framebuffer* currentFb = nullptr;

    void rasterizeBand(unsigned int bandIndex);
    void workerLoop(unsigned int bandIndex);
};
