#include "Framebuffer.h"

bool Framebuffer::test_and_set(int x, int y, float depth, unsigned int color) {
    int index = y * width + x;
    if (depth < depth_buffer[index]) {
        depth_buffer[index] = depth;
        color_buffer[index] = color;
        return true;
    }
    return false;
}
void Framebuffer::clear() {
    std::fill(color_buffer.begin(), color_buffer.end(), 0x000000FF);
    std::fill(depth_buffer.begin(), depth_buffer.end(), 1.0f);
}

/*
these are here so i dont forget the working notcurses setup before i have time to finish this 

    struct notcurses_options opts{};
    struct notcurses* nc = notcurses_init(&opts, stdout);
    if(!nc) {
        std::cerr << "Failed to initialize notcurses\n";
        return 1;
    }

    unsigned int term_width, term_height;
    notcurses_term_dim_yx(nc, &term_height, &term_width);

    struct ncplane* n = notcurses_stdplane(nc);
    ncplane_set_fg_rgb(n, 0xffffff);

    std::vector<Object> scene = loadObjs();
    for(;;) {
        ncplane_erase(n);
        for(auto& o : scene) {

        }
        notcurses_render(nc);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    notcurses_stop(nc);


*/