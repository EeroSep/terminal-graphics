#pragma once
#include "Camera.h"
#include "InputHandler.h"
#include "Vec3.h"

class FlyCam {
    public:
        FlyCam(const Vec3& position, Camera& camera, InputHandler& inputHandler);
        ~FlyCam();

        void update();
    private:
        Camera& camera;
        InputHandler& inputHandler;
        float movementSpeed = 0.1f;
        float mouseDragSensitivity = 0.02f;
        float cameraStepSensitivity = 0.2f;
        int lastMouseX = -1, lastMouseY = -1;
};