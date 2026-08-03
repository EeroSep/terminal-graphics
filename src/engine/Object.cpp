#include "Object.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
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

namespace {
    // OBJ indices are 1-based; negative indices are relative to the vertices seen so far
    int resolveObjIndex(int index, size_t vertexCount) {
        return index < 0 ? static_cast<int>(vertexCount) + index : index - 1;
    }
}

Object Object::loadFromFile(const std::string& filepath, uint32_t color) {
    std::vector<Vec4> vertices;
    std::vector<Face> faces;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Object::loadFromFile: failed to open " << filepath << std::endl;
        return Object(vertices, faces);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string tag;
        ss >> tag;

        if (tag == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.emplace_back(x, y, z, 1.0f);
        } else if (tag == "f") {
            std::vector<int> faceIndices;
            std::string token;
            while (ss >> token) {
                // token may be "v", "v/vt" or "v/vt/vn" - stoi stops at the first '/'
                faceIndices.push_back(resolveObjIndex(std::stoi(token), vertices.size()));
            }
            // Fan-triangulate polygons with more than 3 vertices
            for (size_t i = 1; i + 1 < faceIndices.size(); i++) {
                faces.emplace_back(
                    Vec3(static_cast<float>(faceIndices[0]),
                         static_cast<float>(faceIndices[i]),
                         static_cast<float>(faceIndices[i + 1])),
                    color
                );
            }
        }
        // vt, vn, o, g, s, usemtl, mtllib and comments are ignored
    }

    return Object(vertices, faces);
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
    Mat4 s = scaling(x, y, z);
    setScale(s * scale);
}

void Object::setColor(uint32_t color, int faceIndex) {
    if (faceIndex >= 0 && faceIndex < faces.size()) {
        faces[faceIndex].color = color;
    }
}

