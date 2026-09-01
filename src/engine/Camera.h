#pragma once

#include "../math/Vec3.h"
#include "../math/Mat4.h"

class Camera {
public:
    Camera(const Vec3& pos, float fov, float aspectRatio);
    
    void setPosition(const Vec3& pos);
    void setFov(float fov);
    void setAspectRatio(float aspect);
    
    void rotate(float x, float y);
    void move(float forw, float lateral, float vertical);

    // Yaw alone (no pitch) gives a ground-plane facing direction, e.g. for a
    // walking character that shouldn't move up/down just because the camera
    // is looking up/down.
    float getYaw() const { return yaw; }

    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix() const;
private:
    Vec3 position, forward, up, right;
    float yaw = -1.5708f; // in radians
    float pitch = 0.0f;
    float movementSpeed = 5.0f;
    float mouseSensitivity = 100.0f * 0.01745f; // convert to radians
    float aspectRatio;
    float fov;
    mutable Mat4 viewMatrix;
    mutable bool viewChanged = true;
    Mat4 projectionMatrix;
    Mat4 calculateViewMatrix() const;
    void updateCameraVectors();
};