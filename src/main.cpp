#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <notcurses/notcurses.h>
#include "math/Mat4.h"
#include "math/Vec4.h"
#include "math/Vec3.h"
#include "engine/Framebuffer.h"
#include "engine/Rasterizer.h"
#include "engine/Camera.h"
#include "engine/Renderer.h"
#include "engine/Object.h"
#include "engine/InputHandler.h"

std::vector<Object> loadObjs() {
    //Hardcoded cube :D
    std::vector<Vec4> vertices = {
        Vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        Vec4(0.5f, -0.5f, -0.5f, 1.0f),
        Vec4(0.5f, 0.5f, -0.5f, 1.0f),
        Vec4(-0.5f, 0.5f, -0.5f, 1.0f),
        Vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        Vec4(0.5f, -0.5f, 0.5f, 1.0f),
        Vec4(0.5f, 0.5f, 0.5f, 1.0f),
        Vec4(-0.5f, 0.5f, 0.5f, 1.0f)
    };
    std::vector<Vec3> faces = {
        Vec3(0, 1, 2), Vec3(0, 2, 3), // Back face
        Vec3(4, 5, 6), Vec3(4, 6, 7), // Front face
        Vec3(0, 1, 5), Vec3(0, 5, 4), // Bottom face
        Vec3(2, 3, 7), Vec3(2, 7, 6), // Top face
        Vec3(1, 2, 6), Vec3(1, 6, 5), // Right face
        Vec3(0, 3, 7), Vec3(0, 7, 4)  // Left face
    };
    Object cube(vertices, faces);
    cube.translate(0.0f, 0.0f, -2.0f);
    cube.scaleObject(1.0f, 1.0f, 1.0f);
    //cube.rotate(0.5f, 0.5f, 0.0f);
    std::vector<Object> objects;
    objects.push_back(cube);
    //Object monkey = Object::loadFromFile("assets/monkey.obj");
    //objects.push_back(monkey);
    return objects;
}

int main() {
    struct notcurses_options opts{};
    struct notcurses* nc = notcurses_init(&opts, stdout);
    if (!nc) {
        std::cerr << "Failed to initialize notcurses" << std::endl;
        return -1;
    }
    float deltaTime = 0.0f;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();

    std::vector<Object> scene = loadObjs();

    Renderer renderer(nc);

    // Setting size from renderer
    Framebuffer framebuffer(renderer.getWidth(), renderer.getHeight());

    // Create rasterizer
    Rasterizer rasterizer;

    // Create camera in origin with 90 fov and aspect ratio from renderer
    Camera camera(
        Vec3(0, 0, 0), 
        90.0f,
        static_cast<float>(renderer.getWidth()) / static_cast<float>(renderer.getHeight())
        );

    InputHandler inputHandler(nc);
    bool resizeFlag = false;
    while (true) {
        if (renderer.checkTermSize()) {
            framebuffer.resize(renderer.getWidth(), renderer.getHeight());
            camera.setAspectRatio(static_cast<float>(renderer.getWidth()) / static_cast<float>(renderer.getHeight()));
        }
        framebuffer.clear();
        for (Object& obj : scene) {
            rasterizer.draw(framebuffer, obj, camera);
        }
        renderer.render(&framebuffer);
        // std::vector<uint32_t> inputs = inputHandler.processInput();
        // for (uint32_t input : inputs) {
        //     switch (input) {
        //         case NCKEY_UP:
        //             camera.move(1.0f * deltaTime, 0.0f, 0.0f);
        //             break;
        //         case NCKEY_DOWN:
        //             camera.move(-1.0f * deltaTime, 0.0f, 0.0f);
        //             break;
        //         case NCKEY_LEFT:
        //             camera.move(0.0f, -1.0f * deltaTime, 0.0f);
        //             break;
        //         case NCKEY_RIGHT:
        //             camera.move(0.0f, 1.0f * deltaTime, 0.0f);
        //             break;
        //         default:
        //             break;
        //     }
        // }
        auto now = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;
    }
    notcurses_stop(nc);
    return 0;
}