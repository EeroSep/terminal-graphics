#pragma once
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include <vector>
#include <string>

class Object {
public:
    Object();
    void loadFromFile(const std::string& filename);
    std::vector<Vec4> getVertices() const { return vertices; }
    std::vector<Vec3> getFaces() const { return faces; }
    void setModelMatrix(const Mat4& m) { model = m; }
    Mat4 getModelMatrix() const { return model; }
private:
    std::vector<Vec4> vertices;
    Mat4 model;
    std::vector<Vec3> faces;
};

//WIP 