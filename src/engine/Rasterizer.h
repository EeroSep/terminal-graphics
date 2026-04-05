#pragma once

#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include "Framebuffer.h"
#include "Object.h"



class Rasterizer {
public:
    Rasterizer();

    ~Rasterizer();
    void viewMatrix(const Vec3& eye, const Vec3& center, const Vec3& up);
    void projectionMatrix(float fov, float aspect, float near, float far);
    void rasterize_triangle(const Vec2&v0, const Vec2 &v1, const Vec2 &v2);
    void draw(Framebuffer& fb, const Camera& camera, Object& obj);
private:
    Mat4 view;
    Mat4 projection;
    Framebuffer* fb;
    void init_framebuffer(int width, int height);
};