#include "Object.h"
#include "Texture.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
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
    // OBJ indices are 1-based; negative indices are relative to the elements seen so far
    int resolveObjIndex(int index, size_t count) {
        return index < 0 ? static_cast<int>(count) + index : index - 1;
    }

    struct FaceVertex {
        int vIdx;
        int vtIdx; // -1 if the face didn't specify a texture coordinate
    };

    // token is "v", "v/vt" or "v/vt/vn"
    FaceVertex parseFaceToken(const std::string& token, size_t vertexCount, size_t uvCount) {
        size_t slash1 = token.find('/');
        int vIdx = resolveObjIndex(std::stoi(token.substr(0, slash1)), vertexCount);

        int vtIdx = -1;
        if (slash1 != std::string::npos) {
            size_t slash2 = token.find('/', slash1 + 1);
            std::string vtStr = token.substr(slash1 + 1,
                slash2 == std::string::npos ? std::string::npos : slash2 - slash1 - 1);
            if (!vtStr.empty()) vtIdx = resolveObjIndex(std::stoi(vtStr), uvCount);
        }
        return {vIdx, vtIdx};
    }

    std::string dirOf(const std::string& path) {
        size_t slash = path.find_last_of('/');
        return slash == std::string::npos ? "" : path.substr(0, slash + 1);
    }

    // Parses a Wavefront .mtl file into name -> Material. Only the diffuse
    // color/texture (Kd/map_Kd) are mapped; Ks, Ns, d/Tr, illum etc. are
    // ignored since the renderer has no specular/transparency model.
    std::unordered_map<std::string, Material> loadMtlFile(const std::string& filepath) {
        std::unordered_map<std::string, Material> materials;
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Object::loadFromFile: failed to open mtl file " << filepath << std::endl;
            return materials;
        }

        std::string dir = dirOf(filepath);
        std::string currentName;
        Vec3 albedo(1.0f, 1.0f, 1.0f);
        std::shared_ptr<Texture> texture;

        auto flush = [&]() {
            if (!currentName.empty()) materials.emplace(currentName, Material(albedo, 0.25f, 0.85f, texture));
        };

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string tag;
            ss >> tag;

            if (tag == "newmtl") {
                flush();
                ss >> currentName;
                albedo = Vec3(1.0f, 1.0f, 1.0f);
                texture = nullptr;
            } else if (tag == "Kd") {
                float r, g, b;
                ss >> r >> g >> b;
                albedo = Vec3(r, g, b);
            } else if (tag == "map_Kd") {
                std::string texPath;
                ss >> texPath;
                texture = Texture::loadFromFile(dir + texPath);
            }
        }
        flush();

        return materials;
    }
}

Object Object::loadFromFile(const std::string& filepath, uint32_t color) {
    std::vector<Vec4> vertices;
    std::vector<Vec2> texcoords;
    std::vector<Face> faces;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Object::loadFromFile: failed to open " << filepath << std::endl;
        return Object(vertices, faces);
    }

    std::string dir = dirOf(filepath);
    std::unordered_map<std::string, Material> materials;
    Material currentMaterial(color);

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string tag;
        ss >> tag;

        if (tag == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.emplace_back(x, y, z, 1.0f);
        } else if (tag == "vt") {
            float u, v;
            ss >> u >> v;
            texcoords.emplace_back(u, v);
        } else if (tag == "mtllib") {
            std::string mtlName;
            ss >> mtlName;
            materials = loadMtlFile(dir + mtlName);
        } else if (tag == "usemtl") {
            std::string name;
            ss >> name;
            auto it = materials.find(name);
            if (it != materials.end()) currentMaterial = it->second;
        } else if (tag == "f") {
            std::vector<FaceVertex> faceVerts;
            std::string token;
            while (ss >> token) {
                faceVerts.push_back(parseFaceToken(token, vertices.size(), texcoords.size()));
            }
            // Fan-triangulate polygons with more than 3 vertices
            for (size_t i = 1; i + 1 < faceVerts.size(); i++) {
                const FaceVertex& a = faceVerts[0];
                const FaceVertex& b = faceVerts[i];
                const FaceVertex& c = faceVerts[i + 1];
                Vec2 uvA = a.vtIdx >= 0 ? texcoords[a.vtIdx] : Vec2(0.0f, 0.0f);
                Vec2 uvB = b.vtIdx >= 0 ? texcoords[b.vtIdx] : Vec2(0.0f, 0.0f);
                Vec2 uvC = c.vtIdx >= 0 ? texcoords[c.vtIdx] : Vec2(0.0f, 0.0f);
                faces.emplace_back(
                    Vec3(static_cast<float>(a.vIdx),
                         static_cast<float>(b.vIdx),
                         static_cast<float>(c.vIdx)),
                    uvA, uvB, uvC,
                    currentMaterial
                );
            }
        }
        // vn, o, g, s and comments are ignored
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
        Material& material = faces[faceIndex].material;
        material = Material(color, material.ambient, material.diffuse);
    }
}

