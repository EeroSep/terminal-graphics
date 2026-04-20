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
#include "utility/Colors.h"

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

        std::vector<Face> faces = {
        {Vec3(0, 1, 2), Colors::brown}, {Vec3(0, 2, 3), Colors::brown}, // Back face
        {Vec3(4, 5, 6), Colors::brown}, {Vec3(4, 6, 7), Colors::brown}, // Front face
        {Vec3(0, 1, 5), Colors::brown}, {Vec3(0, 5, 4), Colors::brown}, // Bottom face
        {Vec3(2, 3, 7), Colors::green}, {Vec3(2, 7, 6), Colors::green}, // Top face
        {Vec3(1, 2, 6), Colors::brown}, {Vec3(1, 6, 5), Colors::brown}, // Right face
        {Vec3(0, 3, 7), Colors::brown}, {Vec3(0, 7, 4), Colors::brown}   // Left face
    };
    Object cube(vertices, faces);
    cube.translate(0.0f, -1.0f, -2.0f);

    Object cube2 = cube;
    Object cube3 = cube;
    Object cube4 = cube;
    cube2.translate(1.0f, 1.0f, 0.0f);
    cube3.translate(0.0f, 1.0f, 1.0f);
    cube4.translate(1.0f, 1.0f, 0.0f);
    cube.scaleObject(20.0f, 0.5f, 20.0f);
    std::vector<Object> objects;
    objects.push_back(cube);
    objects.push_back(cube2);
    objects.push_back(cube3);
    objects.push_back(cube4);
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
    int lastMouseX = -1, lastMouseY = -1;
    while (true) {
        if (renderer.checkTermSize()) {
            framebuffer.resize(renderer.getWidth(), renderer.getHeight());
            camera.setAspectRatio(static_cast<float>(renderer.getWidth()) / static_cast<float>(renderer.getHeight()));
        }
        framebuffer.clear();
        for (Object& obj : scene) {
            rasterizer.draw(framebuffer, obj, camera);
        }
        renderer.render(&framebuffer, static_cast<int>(1.0f / deltaTime));

        // Input handling
        std::vector<InputEvent> inputs = inputHandler.processInput();
        for (const InputEvent& inputEvent : inputs) {
            uint32_t input = inputEvent.input;
            if (nckey_mouse_p(input)) {
                if (inputEvent.type == NCTYPE_PRESS) {
                    if (lastMouseX != -1 && lastMouseY != -1) { // take delta when dragging
                        int deltaX = inputEvent.x - lastMouseX;
                        int deltaY = inputEvent.y - lastMouseY;
                        camera.rotate(deltaX * deltaTime, -deltaY * deltaTime * 2.0f);
                    }
                    lastMouseX = inputEvent.x;
                    lastMouseY = inputEvent.y;
                } else if (inputEvent.type == NCTYPE_RELEASE) { // resest last mouse position on release to avoid jumps
                    lastMouseX = -1;
                    lastMouseY = -1;
                }
            }
            switch (input) { // Very nice input handling, will refactor later
                case NCKEY_UP:
                    camera.rotate(0.0f, 1.0f * deltaTime);
                    break;
                case NCKEY_DOWN:
                    camera.rotate(0.0f, -1.0f * deltaTime);
                    break;
                case NCKEY_LEFT:
                    camera.rotate(-1.0f * deltaTime, 0.0f);
                    break;
                case NCKEY_RIGHT:
                    camera.rotate(1.0f * deltaTime, 0.0f);
                    break;
                case 'w':
                    camera.move(1.0f * deltaTime, 0.0f, 0.0f);
                    break;
                case 's':
                    camera.move(-1.0f * deltaTime, 0.0f, 0.0f);
                    break;
                case 'a':
                    camera.move(0.0f, -1.0f * deltaTime, 0.0f);
                    break;
                case 'd':
                    camera.move(0.0f, 1.0f * deltaTime, 0.0f);
                    break;
                default:
                    break;
            }
            
        }
        auto now = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;
    }
    notcurses_stop(nc);
    return 0;
}