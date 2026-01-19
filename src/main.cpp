#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <notcurses/notcurses.h>

int main() {
    glm::ivec2 pos{10, 5};
    std::cout << "Player pos: " << pos.x << ", " << pos.y << "\n";

    struct notcurses_options opts{};
    struct notcurses* nc = notcurses_init(&opts, stdout);
    if(!nc) {
        std::cerr << "Failed to initialize notcurses\n";
        return 1;
    }

    notcurses_stop(nc);
    return 0;
}