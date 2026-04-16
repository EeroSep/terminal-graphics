#include "InputHandler.h"
#include <notcurses/notcurses.h>
#include <vector>

InputHandler::InputHandler(struct notcurses* nc) : nc(nc) {}

InputHandler::~InputHandler() {}

std::vector<uint32_t> InputHandler::processInput() {
    std::vector<uint32_t> inputs(5);
    struct ncinput ni;
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 0};
    uint32_t input;
    while ((input = notcurses_get(nc, &ni, &ts)) != 0) {
        if (id == (uint32_t)-1) break; 
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
            case NCKEY_W:
                    inputs.push_back(NCKEY_W);
                break;
            case NCKEY_A:
                    inputs.push_back(NCKEY_A);
                break;
            case NCKEY_S:
                    inputs.push_back(NCKEY_S);
                break;
            case NCKEY_D:
                    inputs.push_back(NCKEY_D);
                break;
            default:
                break;
        }
    }
    return inputs;
}