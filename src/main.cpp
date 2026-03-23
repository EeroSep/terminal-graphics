#include <iostream>
#include <vector>
#include <notcurses/notcurses.h>
#include "math/Mat4.h"
#include "math/Vec4.h"
#include "math/Vec3.h"
#include "math/graphic_matrices.h"
#include <thread>
#include <chrono>

std::vector<Vec4> generateTorus(float mainRadius, float tubeRadius, int mainSegments, int tubeSegments) {
    std::vector<Vec4> vertices;
    const float PI = 3.14159265359f;

    for (int i = 0; i < mainSegments; ++i) {
        float theta = 2.0f * PI * i / mainSegments;
        
        for (int j = 0; j < tubeSegments; ++j) {
            float phi = 2.0f * PI * j / tubeSegments;

            float x = (mainRadius + tubeRadius * cosf(phi)) * cosf(theta);
            float y = (mainRadius + tubeRadius * cosf(phi)) * sinf(theta);
            float z = tubeRadius * sinf(phi);

            vertices.push_back(Vec4(x, y, z, 1.0f));
        }
    }
    return vertices;
}

struct Object {
    std::vector<Vec4> vertices;
    Mat4 model;
    std::vector<std::pair<int, int>> edges;
};

void drawLine(struct ncplane* n, int x0, int y0, int x1, int y1, const nccell* c) {
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2;

    while (true) {
        ncplane_putc_yx(n, y0, x0, c);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}
int main() {
    struct notcurses_options opts{};
    struct notcurses* nc = notcurses_init(&opts, stdout);
    if(!nc) {
        std::cerr << "Failed to initialize notcurses\n";
        return 1;
    }

    unsigned int term_width, term_height;
    notcurses_term_dim_yx(nc, &term_height, &term_width);
    /*
    Object torus {
        vertices: generateTorus(1.0f, 0.3f, 20, 10),
        model: rotationXYZ(0.2f, 0.1f, 0.0f) *
               scaling(1.0f, 1.0f, 1.0f) *
               translation(0.0f, 0.0f, 0.0f)
    };*/
    Object cube {
        vertices: {
            Vec4(-0.5f, -0.5f, -0.5f, 1.0f),
            Vec4(0.5f, -0.5f, -0.5f, 1.0f),
            Vec4(0.5f, 0.5f, -0.5f, 1.0f),
            Vec4(-0.5f, 0.5f, -0.5f, 1.0f),
            Vec4(-0.5f, -0.5f, 0.5f, 1.0f),
            Vec4(0.5f, -0.5f, 0.5f, 1.0f),
            Vec4(0.5f, 0.5f, 0.5f, 1.0f),
            Vec4(-0.5f, 0.5f, 0.5f, 1.0f)
        },
        model: rotationXYZ(0.2f, 0.1f, 0.0f) *
               scaling(1.0f, 1.0f, 1.0f) *
               translation(2.0f, 0.0f, 0.0f),
        edges: {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        }
    };
    std::vector<Object> objects = {cube};

    nccell c;                       
    nccell_init(&c);                  
    nccell_load_ucs32(nullptr, &c, U'*');
    nccell_set_fg_default(&c);
    nccell_set_bg_default(&c);

    struct ncplane* n = notcurses_stdplane(nc);
    ncplane_set_fg_rgb(n, 0xffffff);
    float rotx = 0.0f, roty = 0.0f;
    Mat4 pers = perspective(90.0f, static_cast<float>(term_width) / static_cast<float>(term_height), 0.1f, 100.0f);
    Mat4 vp = viewport(static_cast<float>(term_width), static_cast<float>(term_height));
    for(;;) {
        ncplane_erase(n);
        rotx += 0.1f;
        roty += 0.1f;
        for(auto& o : objects) {
            o.model = translation(0.0f, 0.0f, -1.0f) * rotationXYZ(rotx, roty, 0.0f) * scaling(1.0f, 1.0f, 1.0f);
            for (auto& v : o.vertices) {

                Vec4 ot = o.model * v;
                ot = pers * ot;
                ot.ndc(); 
                Vec4 screenPos = vp * ot;
                ncplane_putc_yx(n, static_cast<int>(screenPos.y), static_cast<int>(screenPos.x), &c);
                
            }
            for (auto& edge : o.edges) {
                Vec4 v0 = o.model * o.vertices[edge.first];
                Vec4 v1 = o.model * o.vertices[edge.second];
                v0 = pers * v0;
                v1 = pers * v1;
                v0.ndc();
                v1.ndc();
                Vec4 sp0 = vp * v0;
                Vec4 sp1 = vp * v1;
                drawLine(n, static_cast<int>(sp0.x), static_cast<int>(sp0.y), static_cast<int>(sp1.x), static_cast<int>(sp1.y), &c);
            }
        }
        notcurses_render(nc);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    notcurses_stop(nc);
    return 0;
}