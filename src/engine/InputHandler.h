#pragma once
#include <notcurses/notcurses.h>
#include <vector>

class InputHandler {
public:
    InputHandler(struct notcurses* nc);
    ~InputHandler();

    std::vector<uint32_t> processInput();
private:
    std::vector<uint32_t> inputs;
    struct notcurses* nc;
};
