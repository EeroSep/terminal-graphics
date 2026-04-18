#pragma once
#include <notcurses/notcurses.h>
#include <vector>

struct InputEvent {
    uint32_t input;
    int x = -1;
    int y = -1;
    ncintype_e type;
};

class InputHandler {
public:
    InputHandler(struct notcurses* nc);
    ~InputHandler();

    std::vector<InputEvent> processInput();
private:
    std::vector<InputEvent> inputs;
    struct notcurses* nc;
};
