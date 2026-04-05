#pragma once

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Render(const Framebuffer& fb);
    void Clear();
    void calculateDimensions();
    void setBlitter(ncblitter_e blitter) { blit = blitter; }
    void checkTermSize();
    unsigned int getWidth() const { return real_width; }
    unsigned int getHeight() const { return real_height; }

private:
    struct notcurses* nc;
    struct ncplane* pl;
    unsigned int term_width, term_height, real_width, real_height;
    ncblitter_e blit = NCBLIT_1x1;
};