#include "InputHandler.h"
#include <notcurses/notcurses.h>
#include <vector>

InputHandler::InputHandler(struct notcurses* nc, std::mutex& ncMutex) : nc(nc), ncMutex(ncMutex) {
    notcurses_mice_enable(nc, NCMICE_ALL_EVENTS);
}

InputHandler::~InputHandler() {}

std::vector<InputEvent> InputHandler::processInput() {
    std::lock_guard<std::mutex> lock(ncMutex);
    std::vector<InputEvent> inputs;
    struct ncinput ni;
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 0};
    uint32_t input;
    while ((input = notcurses_get(nc, &ts, &ni)) != 0) {
        if (input == (uint32_t)-1) break; 
        InputEvent event;
        event.input = input;
        event.type = ni.evtype;
        if (nckey_mouse_p(input)) {
            event.x = ni.x;
            event.y = ni.y;
        }
        if (input >= 'A' && input <= 'Z') {
            event.input = input + ('a' - 'A'); 
        }
        inputs.push_back(event);
    }
    return inputs;
}