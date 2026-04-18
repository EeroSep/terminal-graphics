#include "Object.h"
#include <cmath>
#include "../math/graphic_matrices.h"

Object::Object(std::vector<Vec4> vertices, std::vector<Vec3> faces) : vertices(vertices), faces(faces) {
    translation = Mat4();
    rotation = Mat4();
    scale = Mat4();
    updateModelMatrix();
}

void Object::loadFromFile(const std::string& filepath) {
    //WIP
}

void Object::rotate(float x, float y, float z) {
    Mat4 rot = rotationXYZ(x, y, z);
    setRotation(rot * rotation);
}

void Object::translate(float x, float y, float z) {
    Mat4 trans = translationM(x, y, z);
    setTranslation(trans * translation);

}

void Object::scaleObject(float x, float y, float z) {
    Mat4 scale = scaling(x, y, z);
    setScale(scale * scale);
}

