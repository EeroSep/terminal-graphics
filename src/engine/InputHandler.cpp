#include "InputHandler.h"
#include <notcurses/notcurses.h>
#include <vector>

InputHandler::InputHandler(struct notcurses* nc) : nc(nc) {}

InputHandler::~InputHandler() {}

std::vector<uint32_t> InputHandler::processInput() {
    std::vector<uint32_t> inputs;
    struct ncinput ni;
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 0};
    uint32_t input;
    while ((input = notcurses_get(nc, &ts, &ni)) != 0) {
        if (input == (uint32_t)-1) break; 
        switch (input) {
            case NCKEY_UP:
                    inputs.push_back(NCKEY_UP);
                break;
            case NCKEY_DOWN:
                    inputs.push_back(NCKEY_DOWN);
                break;
            case NCKEY_LEFT:
                    inputs.push_back(NCKEY_LEFT);
                break;
            case NCKEY_RIGHT:
                    inputs.push_back(NCKEY_RIGHT);
                break;
            case NCKEY_SPACE:
                    inputs.push_back(NCKEY_SPACE);
                break;
            case 'w':
                    inputs.push_back('w');
                break;
            case 'a':
                    inputs.push_back('a');
                break;
            case 's':
                    inputs.push_back('s');
                break;
            case 'd':
                    inputs.push_back('d');
                break;
            case 'W':
                    inputs.push_back('w');
                break;
            case 'A':
                    inputs.push_back('a');
                break;
            case 'S':
                    inputs.push_back('s');
                break;
            case 'D':
                    inputs.push_back('d');
                break;
            default:
                break;
        }
    }
    return inputs;
}