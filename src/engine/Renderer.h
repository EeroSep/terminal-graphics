#pragma once
#include <notcurses/notcurses.h>
#include <mutex>
#include "Framebuffer.h"

class Renderer
{
public:
    // ncMutex must be the same mutex passed to InputHandler: notcurses isn't
    // safe to call concurrently from two threads on the same context, and
    // render()/checkTermSize() serialize themselves against InputHandler's
    // processInput() calls on the raster thread.
    Renderer(struct notcurses* nc, std::mutex& ncMutex);
    ~Renderer();

    void render(const Framebuffer* fb, const int fps);
    void clear();

    void setBlitter(ncblitter_e blitter) { blit = blitter; }
    bool checkTermSize();
    unsigned int getWidth() const { return real_width; }
    unsigned int getHeight() const { return real_height; }

private:
    struct notcurses* nc;
    struct ncplane* game;
    struct ncplane* ui;
    unsigned int term_width, term_height, real_width, real_height;
    ncblitter_e blit = NCBLIT_1x1;
    std::mutex& ncMutex;

    void calculateDimensions();
};