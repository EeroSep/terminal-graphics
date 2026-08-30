#pragma once
#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat4.h"
#include "Material.h"
#include <vector>
#include <string>


struct Face {
    Vec3 indices;
    Vec2 uv[3]; // per-corner texture coordinates, matching order of indices
    Material material;

    Face(Vec3 idx, Vec2 uv0, Vec2 uv1, Vec2 uv2, Material material)
        : indices(idx), uv{uv0, uv1, uv2}, material(material) {}

    // Convenience overload for untextured faces (UVs unused when material.texture is null).
    Face(Vec3 idx, Material material)
        : indices(idx), uv{Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)}, material(material) {}
};

class Object {
public:
    Object(std::vector<Vec4> vertices, std::vector<Face> faces);
    Object(const Object& other);
    static Object loadFromFile(const std::string& filepath, uint32_t color = 0xFFFFFFFFu);
    const std::vector<Vec4>& getVertices() const { return vertices; }
    const std::vector<Face>& getFaces() const { return faces; }
    void setModelMatrix(const Mat4& t, const Mat4& r, const Mat4& s) { translation = t; rotation = r; scale = s; updateModelMatrix(); }
    void setTranslation(const Mat4& t) { translation = t; updateModelMatrix(); }
    void setRotation(const Mat4& r) { rotation = r; updateModelMatrix(); }
    void setScale(const Mat4& s) { scale = s; updateModelMatrix(); }
    void setColor(uint32_t color, int faceIndex);
    void rotate(float x, float y, float z);
    void translate(float x, float y, float z);
    void scaleObject(float x, float y, float z);

    Mat4 getModelMatrix() const { return model; }
private:
    void updateModelMatrix() { model = translation * rotation * scale; }
    std::vector<Vec4> vertices;
    Mat4 translation, rotation, scale;
    Mat4 model;
    std::vector<Face> faces;
};

//WIP 