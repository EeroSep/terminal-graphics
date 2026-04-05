#pragma once

struct Object {
    std::vector<Vec4> vertices;
    Mat4 model;
    std::vector<Vec3> faces;
};