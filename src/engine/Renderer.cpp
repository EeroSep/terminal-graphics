#include "Renderer.h"
#include <notcurses/notcurses.h>

Renderer::Renderer(struct notcurses* nc) : nc(nc) {

    notcurses_term_dim_yx(nc, &term_height, &term_width);
    blit = NCBLIT_3x2;
    calculateDimensions();

    game = notcurses_stdplane(nc);
    ui = notcurses_stdplane(nc);

    ncplane_set_fg_rgb(game, 0xffffff);
    ncplane_set_fg_rgb(ui, 0xffffff);
    ncplane_set_bg_default(ui);
    ncplane_move_top(ui);
}

Renderer::~Renderer() {
}

void Renderer::clear() {
    ncplane_erase(game);
    ncplane_erase(ui);
}
void Renderer::render(const Framebuffer* fb, const int fps) {
    if (!fb || !fb->getColorBuffer()) return;
    ncvisual_options vopts{}; 
    vopts.n = game;
    vopts.blitter = blit;
    vopts.y = 0;
    vopts.x = 0;
    vopts.leny = real_height; 
    vopts.lenx = real_width;
    vopts.scaling = NCSCALE_STRETCH;
    vopts.flags = 0;
    int stride = fb->getWidth() * 4; // 4 bytes per pixel for RGBA
    int result = ncblit_rgba(fb->getColorBuffer(), stride, &vopts); // Render the framebuffer to game plane

    if (result < 0) {
        fprintf(stderr, "Blit failed with code: %d\n", result);
    }
    // Render FPS on UI plane
    ncplane_cursor_move_yx(ui, 0, 0);

    ncplane_printf(ui, "FPS: %d  ", fps);

    notcurses_render(nc);
}

bool Renderer::checkTermSize() {
    unsigned int new_width, new_height;
    notcurses_term_dim_yx(nc, &new_height, &new_width);
    if (new_width != term_width || new_height != term_height) {
        term_width = new_width;
        term_height = new_height;
        calculateDimensions();
        ncplane_resize_realign(game);
        ncplane_resize_realign(ui);
        return true;
    }
    return false;
}

void Renderer::calculateDimensions() {
    switch (blit) {
        case NCBLIT_1x1:
            real_width = term_width;
            real_height = term_height;
            break;
        case NCBLIT_2x1:
            real_width = term_width;
            real_height = term_height  * 2;
            break;
        case NCBLIT_2x2:
            real_width = term_width * 2;
            real_height = term_height * 2;
            break;
        case NCBLIT_3x2:
            real_width = term_width * 2;
            real_height = term_height * 3;
            break;
        case NCBLIT_BRAILLE:
            real_width = term_width * 2;
            real_height = term_height * 4;
            break;
        default:
            real_width = term_width;
            real_height = term_height;
    }

}
    

    