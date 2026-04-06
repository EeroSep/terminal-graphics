#pragma once
// Maybe like this?

class InputHandler {
public:
    InputHandler(struct notcurses& nc);
    ~InputHandler();

    void processInput();
private:
    struct notcurses* nc;
};

// WIP