#include "Player.h"
#include "Mat4.h"
#include "Vec4.h"
#include <cmath>

namespace {
// Ericson, "Real-Time Collision Detection" 5.1.5 — closest point on triangle
// abc to point p, via its barycentric region.
Vec3 closestPointOnTriangle(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c) {
    Vec3 ab = b - a, ac = c - a, ap = p - a;
    float d1 = ab.dot(ap), d2 = ac.dot(ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return a;

    Vec3 bp = p - b;
    float d3 = ab.dot(bp), d4 = ac.dot(bp);
    if (d3 >= 0.0f && d4 <= d3) return b;

    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        float v = d1 / (d1 - d3);
        return a + ab * v;
    }

    Vec3 cp = p - c;
    float d5 = ab.dot(cp), d6 = ac.dot(cp);
    if (d6 >= 0.0f && d5 <= d6) return c;

    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        float w = d2 / (d2 - d6);
        return a + ac * w;
    }

    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + (c - b) * w;
    }

    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return a + ab * v + ac * w;
}
}

Player::Player(const Vec3& position, Camera& camera, InputHandler& inputHandler, const Object& map)
    : camera(camera), inputHandler(inputHandler), position(position) {
    Mat4 model = map.getModelMatrix();
    const std::vector<Vec4>& vertices = map.getVertices();
    for (const Face& face : map.getFaces()) {
        Vec3 a = Vec3(model * vertices[static_cast<int>(face.indices.x)]);
        Vec3 b = Vec3(model * vertices[static_cast<int>(face.indices.y)]);
        Vec3 c = Vec3(model * vertices[static_cast<int>(face.indices.z)]);
        mapTriangles.push_back({a, b, c});
    }
    camera.setPosition(position + Vec3(0.0f, eyeHeight, 0.0f));
}

Player::~Player() {
}

void Player::resolveCollisions() {
    Vec3 center = position + Vec3(0.0f, radius, 0.0f);
    grounded = false;
    // A few iterations so a corner touching two triangles at once (e.g. a
    // wall meeting the floor) settles into a consistent resolved position
    // instead of only reacting to whichever triangle was tested last.
    for (int iter = 0; iter < 4; iter++) {
        for (const Triangle& tri : mapTriangles) {
            Vec3 closest = closestPointOnTriangle(center, tri.a, tri.b, tri.c);
            Vec3 diff = center - closest;
            float dist = diff.length();
            if (dist >= radius || dist <= 1e-6f) continue;

            Vec3 pushDir = diff * (1.0f / dist);
            center = center + pushDir * (radius - dist);

            if (pushDir.y > 0.5f) {
                grounded = true;
                if (verticalVelocity < 0.0f) verticalVelocity = 0.0f;
            } else if (pushDir.y < -0.5f) {
                if (verticalVelocity > 0.0f) verticalVelocity = 0.0f;
            }
        }
    }
    position = center - Vec3(0.0f, radius, 0.0f);
}

void Player::update(float deltaTime) {
    float yaw = camera.getYaw();
    // Flat (pitch-free) facing direction so looking up/down doesn't tilt
    // ground movement, matching Camera::updateCameraVectors at pitch == 0.
    Vec3 flatForward = Vec3(cosf(yaw), 0.0f, sinf(yaw)).normalize();
    Vec3 flatRight = flatForward.cross(Vec3(0.0f, 1.0f, 0.0f)).normalize();

    std::vector<InputEvent> inputs = inputHandler.processInput();
    for (const InputEvent& inputEvent : inputs) {
        uint32_t input = inputEvent.input;
        if (nckey_mouse_p(input)) {
            if (inputEvent.type == NCTYPE_PRESS) {
                if (lastMouseX != -1 && lastMouseY != -1) {
                    int deltaX = inputEvent.x - lastMouseX;
                    int deltaY = inputEvent.y - lastMouseY;
                    camera.rotate(deltaX * mouseDragSensitivity, -deltaY * mouseDragSensitivity * 2.0f);
                }
                lastMouseX = inputEvent.x;
                lastMouseY = inputEvent.y;
            } else if (inputEvent.type == NCTYPE_RELEASE) {
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
                position = position + flatForward * movementSpeed;
                break;
            case 's':
                position = position - flatForward * movementSpeed;
                break;
            case 'a':
                position = position - flatRight * movementSpeed;
                break;
            case 'd':
                position = position + flatRight * movementSpeed;
                break;
            case ' ':
                if (grounded) verticalVelocity = jumpSpeed;
                break;
            default:
                break;
        }
    }

    verticalVelocity += gravity * deltaTime;
    position.y += verticalVelocity * deltaTime;

    resolveCollisions();

    camera.setPosition(position + Vec3(0.0f, eyeHeight, 0.0f));
}
