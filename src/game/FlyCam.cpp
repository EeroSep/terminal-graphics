#include "FlyCam.h"

FlyCam::FlyCam(const Vec3& position, Camera& camera, InputHandler& inputHandler)
    : camera(camera), inputHandler(inputHandler) {
    this->camera.setPosition(position);
}

FlyCam::~FlyCam() {
}

void FlyCam::update() {
    std::vector<InputEvent> inputs = inputHandler.processInput();
    for (const InputEvent& inputEvent : inputs) {
        uint32_t input = inputEvent.input;
        if (nckey_mouse_p(input)) {
            if (inputEvent.type == NCTYPE_PRESS) {
                if (lastMouseX != -1 && lastMouseY != -1) { // take delta when dragging
                    int deltaX = inputEvent.x - lastMouseX;
                    int deltaY = inputEvent.y - lastMouseY;
                    camera.rotate(deltaX * mouseDragSensitivity, -deltaY * mouseDragSensitivity * 2.0f);
                }
                lastMouseX = inputEvent.x;
                lastMouseY = inputEvent.y;
            } else if (inputEvent.type == NCTYPE_RELEASE) { // resest last mouse position on release to avoid jumps
                lastMouseX = -1;
                lastMouseY = -1;
            }
            continue;
        }
        switch (input) {
            case NCKEY_UP:
                camera.rotate(0.0f, cameraStepSensitivity);
                break;
            case NCKEY_DOWN:
                camera.rotate(0.0f, -cameraStepSensitivity);
                break;
            case NCKEY_LEFT:
                camera.rotate(-cameraStepSensitivity, 0.0f);
                break;
            case NCKEY_RIGHT:
                camera.rotate(cameraStepSensitivity, 0.0f);
                break;
            case 'w':
                camera.move(movementSpeed, 0.0f, 0.0f);
                break;
            case 's':
                camera.move(-movementSpeed, 0.0f, 0.0f);
                break;
            case 'a':
                camera.move(0.0f, -movementSpeed, 0.0f);
                break;
            case 'd':
                camera.move(0.0f, movementSpeed, 0.0f);
                break;
            default:
                break;
        }
    }
}
