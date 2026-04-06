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

    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix() const;
private:
    Vec3 position, forward, up, right;
    float yaw = -1.5708f; // in radians
    float pitch = 0.0f;
    float movementSpeed = 1.0f;
    float mouseSensitivity = 0.1f * 0.01745f; // convert to radians
    float aspectRatio;
    float fov;
    mutable Mat4 viewMatrix;
    mutable bool viewChanged = true;
    Mat4 projectionMatrix;
    Mat4 calculateViewMatrix() const;
    void updateCameraVectors();
};