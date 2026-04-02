#include <iostream>
#include <vector>
#include <notcurses/notcurses.h>
#include "math/Mat4.h"
#include "math/Vec4.h"
#include "math/Vec3.h"
#include "math/graphic_matrices.h"
#include <thread>
#include <chrono>
#include "engine/Framebuffer.h"
#include "engine/Rasterizer.h"
#include "Camera.h"

struct Object {
    std::vector<Vec4> vertices;
    Mat4 model;
    std::vector<Vec3> faces;
};

std::vector<Object> loadObjs() {
    //Hardcoded cube :D
    Object cube;
    cube.vertices = {
        Vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        Vec4(0.5f, -0.5f, -0.5f, 1.0f),
        Vec4(0.5f, 0.5f, -0.5f, 1.0f),
        Vec4(-0.5f, 0.5f, -0.5f, 1.0f),
        Vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        Vec4(0.5f, -0.5f, 0.5f, 1.0f),
        Vec4(0.5f, 0.5f, 0.5f, 1.0f),
        Vec4(-0.5f, 0.5f, 0.5f, 1.0f)
    };
    cube.faces = {
        Vec3(0, 1, 2), Vec3(0, 2, 3), // Back face
        Vec3(4, 5, 6), Vec3(4, 6, 7), // Front face
        Vec3(0, 1, 5), Vec3(0, 5, 4), // Bottom face
        Vec3(2, 3, 7), Vec3(2, 7, 6), // Top face
        Vec3(1, 2, 6), Vec3(1, 6, 5), // Right face
        Vec3(0, 3, 7), Vec3(0, 7, 4)  // Left face
    };
    // TODO load everything from obj folder
    return {cube};
}

int main() {

    return 0;
}