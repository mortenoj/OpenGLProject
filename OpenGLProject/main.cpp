#include "game.hpp"


int main() {
    Game game("Learning OpenGL", 640*2, 480*2, 3, 3, true);

    // Main loop
    while (!game.getWindowShouldClose()) {
        // Update input ---
        game.update();
        game.render();
    }

    return EXIT_SUCCESS;
}
