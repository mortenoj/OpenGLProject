#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertex.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "primitives.hpp"
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
