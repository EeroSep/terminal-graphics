#pragma once
#include <notcurses/notcurses.h>
#include <vector>
#include <mutex>

struct InputEvent {
    uint32_t input;
    int x = -1;
    int y = -1;
    ncintype_e type;
};

class InputHandler {
public:
    // ncMutex must be the same mutex passed to Renderer: notcurses isn't
    // safe to call concurrently from two threads on the same context, and
    // processInput() serializes itself against Renderer's render()/
    // checkTermSize() calls on the render thread.
    InputHandler(struct notcurses* nc, std::mutex& ncMutex);
    ~InputHandler();

    std::vector<InputEvent> processInput();
private:
    std::vector<InputEvent> inputs;
    struct notcurses* nc;
    std::mutex& ncMutex;
};
