#include "Camera.h"
#include "../math/graphic_matrices.h"
#include "../math/Vec3.h"

Camera::Camera() : position(0, 0, 0), target(0, 0, -1), up(0, 1, 0) {}

Camera::Camera(const Vec3& position, const Vec3& target, const Vec3& up) 
    : position(position), target(target), up(up) {}

Mat4 Camera::getViewMatrix() const {

}