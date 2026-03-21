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
};

int main() {
    struct notcurses_options opts{};
    struct notcurses* nc = notcurses_init(&opts, stdout);
    if(!nc) {
        std::cerr << "Failed to initialize notcurses\n";
        return 1;
    }

    unsigned int term_width, term_height;
    notcurses_term_dim_yx(nc, &term_height, &term_width);
    Object torus {
        vertices: generateTorus(1.0f, 0.3f, 20, 10),
        model: rotationXYZ(0.2f, 0.1f, 0.0f) *
               scaling(1.0f, 1.0f, 1.0f) *
               translation(0.0f, 0.0f, 0.0f)
    };
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
               translation(2.0f, 0.0f, 0.0f)
    };
    std::vector<Object> objects = {torus};

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
        rotx += 0.01f;
        roty += 0.01f;
        for(auto& o : objects) {
            o.model = translation(0.0f, 0.0f, -1.0f) * rotationXYZ(rotx, roty, 0.0f) * scaling(1.0f, 1.0f, 1.0f);
            for (auto& v : o.vertices) {

                Vec4 ot = o.model * v;
                ot = pers * ot;
                ot.ndc(); 
                Vec4 screenPos = vp * ot;
                ncplane_putc_yx(n, static_cast<int>(screenPos.y), static_cast<int>(screenPos.x), &c);
            }
        }
        notcurses_render(nc);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    notcurses_stop(nc);
    return 0;
}