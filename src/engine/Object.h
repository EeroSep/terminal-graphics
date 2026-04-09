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
    void setModelMatrix(const Mat4& t, const Mat4& r, const Mat4& s) { translation = t; rotation = r; scale = s; updateModelMatrix(); }
    void setTranslation(const Mat4& t) { translation = t; updateModelMatrix(); }
    void setRotation(const Mat4& r) { rotation = r; updateModelMatrix(); }
    void setScale(const Mat4& s) { scale = s; updateModelMatrix(); }

    void rotate(float x, float y, float z);
    void translate(float x, float y, float z);
    void scaleObject(float x, float y, float z);

    Mat4 getModelMatrix() const { return model; }
private:
    void updateModelMatrix() { model = translation * rotation * scale; }
    std::vector<Vec4> vertices;
    Mat4 translation, rotation, scale;
    Mat4 model;
    std::vector<Vec3> faces;
};

//WIP 