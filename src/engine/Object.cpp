#include "Object.h"
#include <cmath>
#include "../math/graphic_matrices.h"

Object::Object(std::vector<Vec4> vertices, std::vector<Face> faces) : vertices(vertices), faces(faces) {
    translation = Mat4();
    rotation = Mat4();
    scale = Mat4();
    updateModelMatrix();
}

Object::Object(const Object& other) : vertices(other.vertices), faces(other.faces), translation(other.translation), rotation(other.rotation), scale(other.scale) {
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

void Object::setColor(uint32_t color, int faceIndex) {
    if (faceIndex >= 0 && faceIndex < faces.size()) {
        faces[faceIndex].color = color;
    }
}

