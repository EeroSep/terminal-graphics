#include "Renderer.h"
#include <notcurses/notcurses.h>

Renderer::Renderer(struct notcurses* nc) : nc(nc) {

    notcurses_term_dim_yx(nc, &term_height, &term_width);
    pl = notcurses_stdplane(nc);
    ncplane_set_fg_rgb(pl, 0xffffff);
}

Renderer::~Renderer() {
    notcurses_stop(nc);
}

void Renderer::Clear() {
    ncplane_erase(pl);
}
void Renderer::render(const Framebuffer* fb) {
    ncblit_rgba(fb->getColorBuffer(), ); // !!!!!
    notcurses_render(nc);
}
void Renderer::checkTermSize() {
    unsigned int new_width, new_height;
    notcurses_term_dim_yx(nc, &new_height, &new_width);
    if (new_width != term_width || new_height != term_height) {
        term_width = new_width;
        term_height = new_height;
        calculateDimensions();
    }
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
        case NCBLIT_1x2:
            real_width = term_width * 2;
            real_height = term_height;
            break;
        case NCBLIT_2x2:
            real_width = term_width * 2;
            real_height = term_height * 2;
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
    

    