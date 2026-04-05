#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <notcurses/notcurses.h>
#include "math/Mat4.h"
#include "math/Vec4.h"
#include "math/Vec3.h"
#include "math/graphic_matrices.h"
#include "engine/Framebuffer.h"
#include "engine/Rasterizer.h"
#include "engine/Camera.h"
#include "engine/Renderer.h"
#include "engine/Object.h"

struct GlobalState {
    float deltaTime;
    float lastFrameTime;
    struct notcurses* nc;
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
    // ? Obj class with loading function instead of struct ? 
    return {cube};
}

int main() {
    // Complete refactor in progress
    std::vector<Object> scene = loadObjs();
    Renderer renderer;
    Rasterizer rasterizer;
    while (true) {
        
    }
    return 0;
}