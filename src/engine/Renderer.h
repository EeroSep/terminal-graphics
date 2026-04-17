#pragma once
#include <notcurses/notcurses.h>
#include "Framebuffer.h"

class Renderer
{
public:
    Renderer(struct notcurses* nc);
    ~Renderer();

    void render(const Framebuffer* fb);
    void clear();
   
    void setBlitter(ncblitter_e blitter) { blit = blitter; }
    bool checkTermSize();
    unsigned int getWidth() const { return real_width; }
    unsigned int getHeight() const { return real_height; }

private:
    struct notcurses* nc;
    struct ncplane* pl;
    unsigned int term_width, term_height, real_width, real_height;
    ncblitter_e blit = NCBLIT_1x1;
    void calculateDimensions();
};