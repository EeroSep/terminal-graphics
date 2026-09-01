#pragma once
#include <vector>
#include "Camera.h"
#include "InputHandler.h"
#include "Object.h"
#include "Vec3.h"

class Player {
    public:
        Player(const Vec3& position, Camera& camera, InputHandler& inputHandler, const Object& map);
        ~Player();

        void update(float deltaTime);

    private:
        struct Triangle {
            Vec3 a, b, c;
        };

        Camera& camera;
        InputHandler& inputHandler;
        std::vector<Triangle> mapTriangles;

        Vec3 position;       // feet position (ground contact point)
        float verticalVelocity = 0.0f;
        bool grounded = false;

        float radius = 0.4f;         // collision sphere radius
        float eyeHeight = 1.6f;      // camera offset above feet
        float movementSpeed = 0.15f; // horizontal distance per key-repeat step
        float mouseDragSensitivity = 0.02f;
        float cameraStepSensitivity = 0.2f;
        float gravity = -9.8f;
        float jumpSpeed = 6.0f;
        int lastMouseX = -1, lastMouseY = -1;

        void resolveCollisions();
};
