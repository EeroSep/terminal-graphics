#include <iostream>
#include <vector>
#include <notcurses/notcurses.h>
#include "math/Mat4.h"
#include "math/Vec4.h"
#include "math/Vec3.h"
#include "math/graphic_matrices.h"
#include <thread>
#include <chrono>

struct ScreenPos {
    int x;
    int y;
};

ScreenPos project(const Vec3& v, int width, int height, float scale = 10.0f) {
    // Simple orthographic projection, center in terminal
    int cx = width / 2;
    int cy = height / 2;
    return {
        cx + static_cast<int>(v.x * scale),
        cy - static_cast<int>(v.y * scale)  // y flipped for terminal coords
    };
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

    // Model transformation
    Mat4 model = rotationXYZ(0.2f, 0.1f, 0.0f) *
                 scaling(1.0f, 1.0f, 1.0f) *
                 translation(0.0f, 0.0f, 0.0f);

    // Cube vertices
    std::vector<Vec4> cube {
        Vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        Vec4(0.5f, -0.5f, -0.5f, 1.0f),
        Vec4(0.5f, 0.5f, -0.5f, 1.0f),
        Vec4(-0.5f, 0.5f, -0.5f, 1.0f),
        Vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        Vec4(0.5f, -0.5f, 0.5f, 1.0f),
        Vec4(0.5f, 0.5f, 0.5f, 1.0f),
        Vec4(-0.5f, 0.5f, 0.5f, 1.0f)
    };
    nccell c;                       
    nccell_init(&c);                  
    nccell_load_ucs32(nullptr, &c, U'*');
    nccell_set_fg_default(&c);
    nccell_set_bg_default(&c);
    struct ncplane* n = notcurses_stdplane(nc);
    ncplane_set_fg_rgb(n, 0xffffff);
    // Main loop
    float rotx = 0.0f, roty = 0.0f;
    for(;;) {
        ncplane_erase(n);
        model = rotationXYZ(rotx += 0.1f, roty += 0.05f, 0.0f) *
                 scaling(1.0f, 1.0f, 1.0f) *
                 translation(0.0f, 0.0f, 0.0f);
        for(auto& v : cube) {
            Vec4 vt = model * v;
            Vec3 projected(vt);  
            ScreenPos p = project(projected, term_width, term_height);

            if(p.x >= 0 && p.x < (int)term_width && p.y >= 0 && p.y < (int)term_height) {
                ncplane_putc_yx(n, p.y, p.x, &c); 
            }
        }
        notcurses_render(nc);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    notcurses_stop(nc);
    return 0;
}