#pragma once

#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"

class Camera {
public:
    Camera();
    Camera(const Vec3& position, const Vec3& target, const Vec3& up);
    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) const;
private:
    Vec3 position;
    Vec3 target;
    Vec3 up;
};