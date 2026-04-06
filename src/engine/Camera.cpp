#include "Camera.h"
#include "../math/graphic_matrices.h"
#include "../math/Vec3.h"
#include "../math/Mat4.h"
#include <cmath>

Camera::Camera(const Vec3& pos, float fov, float aspectRatio) 
    : position(pos), aspectRatio(aspectRatio), fov(fov) {
    projectionMatrix = perspective(fov, aspectRatio, 0.1f, 100.0f);
    updateCameraVectors();
}

void Camera::setPosition(const Vec3& pos) {
    position = pos;
    viewChanged = true;
    updateCameraVectors();
}

void Camera::setFov(float fov) {
    this->fov = fov;
    projectionMatrix = perspective(fov, aspectRatio, 0.1f, 100.0f);
}

void Camera::setAspectRatio(float aspect) {
    aspectRatio = aspect;
    projectionMatrix = perspective(fov, aspectRatio, 0.1f, 100.0f);
}

void Camera::rotate(float x, float y) {
    yaw += x * mouseSensitivity;
    pitch += y * mouseSensitivity;
    if (pitch > 1.553f) pitch = 1.553f; // 89d in radians
    if (pitch < -1.553f) pitch = -1.553f;
    updateCameraVectors();
}

void Camera::move(float forw, float lateral, float vertical) {
    position = position + (forward * forw * movementSpeed)
                        + (right * lateral * movementSpeed)
                        + (Vec3(0.0f, 1.0f, 0.0f) * vertical * movementSpeed);
    viewChanged = true;
}

Mat4 Camera::calculateViewMatrix() const {
    float m[16] = {
         right.x,  right.y,  right.z, -right.dot(position),
         up.x,  up.y,  up.z, -up.dot(position),
        -forward.x, -forward.y, -forward.z,  forward.dot(position), 
         0.0f, 0.0f, 0.0f, 1.0f
    };
    return Mat4(m); 
}

Mat4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

Mat4 Camera::getViewMatrix() const {
    if(viewChanged) { // Only recalculate if something changed
        viewMatrix = calculateViewMatrix();
        viewChanged = false;
    }
    return viewMatrix;
}

Mat4 perspective(float fov, float aspect, float near, float far) {
    float f = 1.0f / tanf(fov / 2.0f);
    float m[16] = {
        f / aspect, 0.0f, 0.0f,  0.0f, 
        0.0f,       f,    0.0f,  0.0f,
        0.0f,       0.0f, -(far + near) / (far - near), -(2 * far * near) / (far - near),
        0.0f,       0.0f, -1.0f, 0.0f
    };
    return Mat4(m);
}

void Camera::updateCameraVectors() {
    Vec3 newTarget;
    newTarget.x = cosf(yaw) * cosf(pitch);
    newTarget.y = sinf(pitch);
    newTarget.z = sinf(yaw) * cosf(pitch);
    forward = newTarget.normalize();
    right = forward.cross(Vec3(0.0f, 1.0f, 0.0f)).normalize();
    up = right.cross(forward).normalize();
    viewChanged = true;
}

 