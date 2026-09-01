#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
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
#include "engine/Light.h"
#include "utility/Colors.h"
#include "game/FlyCam.h"
#include "game/Player.h"

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
        {Vec3(0, 2, 1), Colors::brown}, {Vec3(0, 3, 2), Colors::brown}, // Back face
        {Vec3(4, 5, 6), Colors::brown}, {Vec3(4, 6, 7), Colors::brown}, // Front face
        {Vec3(0, 1, 5), Colors::brown}, {Vec3(0, 5, 4), Colors::brown}, // Bottom face
        {Vec3(2, 3, 7), Colors::green}, {Vec3(2, 7, 6), Colors::green}, // Top face
        {Vec3(1, 2, 6), Colors::brown}, {Vec3(1, 6, 5), Colors::brown}, // Right face
        {Vec3(0, 7, 3), Colors::brown}, {Vec3(0, 4, 7), Colors::brown}   // Left face
    };
    Object cube(vertices, faces);
    cube.translate(0.0f, -1.0f, -2.0f);

    Object cube2 = cube;
    Object cube3 = cube;
    Object cube4 = cube;
    cube2.translate(1.0f, 1.0f, 0.0f);
    cube3.translate(0.0f, 1.0f, 1.0f);
    cube4.translate(1.0f, 1.0f, 0.0f);
    std::vector<Object> objects;
    objects.push_back(cube);
    objects.push_back(cube2);
    objects.push_back(cube3);
    objects.push_back(cube4);
    Object pyramid = Object::loadFromFile("assets/pyramid.obj", Colors::purple);
    pyramid.translate(-2.0f, -0.5f, -2.0f);
    objects.push_back(pyramid);

    Object monkey = Object::loadFromFile("assets/monkey.obj", Colors::purple);
    monkey.translate(2.0f, -0.5f, 2.0f);
    objects.push_back(monkey);

    Object teapot = Object::loadFromFile("assets/teapot.obj", Colors::red);
    teapot.translate(-2.0f, -0.5f, 2.0f);
    objects.push_back(teapot);
    Object map = Object::loadFromFile("assets/map.obj", Colors::white);
    map.rotate(0.0f, 0.0f, -1.5707963f); // 90 degrees, in radians
    map.scaleObject(50.0f, 50.0f, 50.0f);
    objects.push_back(map);
    return objects;
}

int main() {
    struct notcurses_options opts{};
    struct notcurses* nc = notcurses_init(&opts, stdout);
    if (!nc) {
        std::cerr << "Failed to initialize notcurses" << std::endl;
        return -1;
    }
    // Non-zero so the first frame's 1/deltaTime (used below to seed the FPS
    // EMA) isn't infinite.
    float deltaTime = 1.0f / 60.0f;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    float smoothedFps = 0.0f;
    const float fpsSmoothing = 0.1f; // lower = smoother/slower to react

    std::vector<Object> scene = loadObjs();

    // notcurses itself isn't safe to call concurrently from two threads on
    // the same context, so Renderer and InputHandler each lock this around
    // their own notcurses calls (render()/checkTermSize() vs processInput()).
    std::mutex ncMutex;

    Renderer renderer(nc, ncMutex);

    // Setting size from renderer
    Framebuffer framebuffer(renderer.getWidth(), renderer.getHeight());
    framebuffer.setClearColor(Colors::rgba(135, 206, 235, 255)); // sky blue
    //framebuffer.setClearColor(Colors::black); 
    Rasterizer rasterizer;

    // Create camera in origin with 90 fov and aspect ratio from renderer
    Camera camera(
        Vec3(0, 0, 0), 
        90.0f,
        static_cast<float>(renderer.getWidth()) / static_cast<float>(renderer.getHeight())
        );

    Light light(Vec3(0.4f, 1.0f, 0.3f));

    InputHandler inputHandler(nc, ncMutex);
    bool resizeFlag = false;
    // FlyCam flyCam(Vec3(0, 0, 0), camera, inputHandler);
    // loadObjs() always pushes the map last; spawn a couple units up so
    // gravity settles the player onto the floor instead of starting embedded
    // in it if y=0 isn't exactly ground level.
    Player player(Vec3(5, -8.0f, 5), camera, inputHandler, scene.back());

    // Handoff between the raster thread (this loop) and the render thread below.
    // frameReady: raster has swapped a finished frame into the front buffer, render should pick it up.
    // bufferFree: render has finished with the front buffer, raster may swap again.
    std::mutex mtx;
    std::condition_variable cv;
    bool frameReady = false;
    bool bufferFree = true;
    std::atomic<int> fps{0};

    std::thread renderThread([&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&] { return frameReady; });
            frameReady = false;
            lock.unlock();

            renderer.render(&framebuffer, fps.load());

            {
                std::lock_guard<std::mutex> lock2(mtx);
                bufferFree = true;
            }
            cv.notify_one();
        }
    });

    while (true) {
        // Safe unlocked: this only ever touches the current back buffer,
        // which the render thread never reads.
        framebuffer.clear();
        for (Object& obj : scene) {
            rasterizer.draw(framebuffer, obj, camera, light);
        }
        // Rasterizes every triangle queued above across the thread pool,
        // then clears the queue for the next frame.
        rasterizer.flush(framebuffer);

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&] { return bufferFree; });
            bool resized = renderer.checkTermSize();
            if (resized) {
                framebuffer.resize(renderer.getWidth(), renderer.getHeight());
                camera.setAspectRatio(static_cast<float>(renderer.getWidth()) / static_cast<float>(renderer.getHeight()));
            }
            framebuffer.swapBuffer();
            bufferFree = false;
            frameReady = true;
        }
        cv.notify_one();
        // Exponential moving average: an instantaneous 1/deltaTime readout
        // amplifies small per-frame jitter into a distractingly noisy number.
        smoothedFps = smoothedFps + fpsSmoothing * ((1.0f / deltaTime) - smoothedFps);
        fps.store(static_cast<int>(smoothedFps));

        // Input handling
        // flyCam.update();
        player.update(deltaTime);

        auto now = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;
    }
    notcurses_stop(nc);
    return 0;
}