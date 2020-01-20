#include "game.hpp"

// Constructors/Destructor
Game::Game(
        const char* title,
        const int width, const int height,
        const int glMajVer, const int glMinVer,
        bool resizable
        ) : WINDOW_WIDTH(width),
            WINDOW_HEIGHT(height),
            GL_VERSION_MAJOR(glMajVer),
            GL_VERSION_MINOR(glMinVer) {

    this->window = NULL;
    this->frameBufferWidth = this->WINDOW_WIDTH;
    this->frameBufferHeight = this->WINDOW_HEIGHT;

    // View Matrix
    this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->cameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    // Projection Matrix
    this->fov = 90.0f;
    this->zNear = 0.1f;
    this->zFar = 1000.0f;

    this->initGLFW();
    this->initWindow(title, resizable);
    this->initGLEW();
    this->initOpenGLOptions();
    this->initMatrices();
    this->initShaders();
    this->initTextures();
    this->initMaterials();
    this->initMeshes();
    this->initLights();
    this->initUniforms();
}

Game::~Game() {
    glfwDestroyWindow(this->window);
    glfwTerminate();

    for (size_t i = 0; i < this->shaders.size(); i++) { delete this->shaders[i]; }
    for (size_t i = 0; i < this->textures.size(); i++) { delete this->textures[i]; }
    for (size_t i = 0; i < this->materials.size(); i++) { delete this->materials[i]; }
    for (size_t i = 0; i < this->meshes.size(); i++) { delete this->meshes[i]; }
    for (size_t i = 0; i < this->lights.size(); i++) { delete this->lights[i]; }
}

// Private functions
void Game::initGLFW() {
    if (glfwInit() == GLFW_FALSE) {
        fprintf(stderr, "GAME: Failed to init GLFW.\n");
		glfwTerminate();
    }
}

// Init function initializes the game
void Game::initWindow(const char* title, bool resizable) {

    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);
	if (this->window == NULL) {
		fprintf(stderr, "MAIN: Failed to open GLFW window.\n");
		glfwTerminate();
		return;
	}

    glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
    glfwSetFramebufferSizeCallback(this->window, Game::framebufferResizeCallback);
    glfwMakeContextCurrent(this->window); // Important for GLEW
}

void Game::initGLEW() {
    // Init GLEW (needs window and opengl context)
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "MAIN: Failed to initialize GLEW\n");
        glfwTerminate();
    }
}

void Game::initOpenGLOptions() {
    glEnable(GL_DEPTH_TEST); // Enables Z coordinate

    glEnable(GL_CULL_FACE); // Removes unnecessary faces
    glCullFace(GL_BACK); // Cull backsides of triangles
    glFrontFace(GL_CCW); // Set OpenGL to render triangles counter clock-wise

    glEnable(GL_BLEND); // Blending of colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // WIREFRAME
}

void Game::initMatrices() {
    this->viewMatrix = glm::mat4(1.0f);
    this->viewMatrix = glm::lookAt(
            this->cameraPosition,
            this->cameraPosition + this->cameraFront,
            this->worldUp
        );

    this->projectionMatrix = glm::mat4(1.0f);
    this->projectionMatrix = glm::perspective(
            glm::radians(this->fov),
            static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
            zNear, zFar
        );
}


void Game::initShaders() {
    this->shaders.push_back(
            new Shader(
                this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
                "res/shaders/core.vert", "res/shaders/core.frag"
            )
        );
}

void Game::initTextures() {
    this->textures.push_back(new Texture("res/images/cat.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("res/images/crate.png", GL_TEXTURE_2D));
}

void Game::initMaterials() {
    this->materials.push_back(
            new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 0, 1)
        );
}

void Game::initMeshes() {
    Quad q = Quad();

    this->meshes.push_back(
            new Mesh(&q,
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                glm::vec3(1.0f)
            )
        );
}

void Game::initLights() {
    this->lights.push_back(new glm::vec3(0.0f, 0.0f, 1.0f));
}

void Game::initUniforms() {
    this->shaders[SHADER_CORE_PROGRAM]->setMat4f(this->viewMatrix, "ViewMatrix");
    this->shaders[SHADER_CORE_PROGRAM]->setMat4f(this->projectionMatrix, "ProjectionMatrix");

    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->cameraPosition, "cameraPos");
}

void Game::updateUniforms() {
    // Update uniforms
    this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

    // Update frame buffer to get new width and height 
    glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);

    // Update projection matrix in case of resizing of window (should be done only when window is resizing)
    this->projectionMatrix = glm::mat4(1.0f);
    this->projectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
        zNear,
        zFar
    );

    this->shaders[SHADER_CORE_PROGRAM]->setMat4f(this->projectionMatrix, "ProjectionMatrix");
}

// Public functions

// Update function
void Game::update() {
    // Update input
    glfwPollEvents();
    updateInput(this->window);
    updateInput(this->window, *this->meshes[MESH_QUAD]);
}

// Render function
void Game::render() {
    //  Clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Update uniforms
    this->updateUniforms();

    // Use a program
    this->shaders[SHADER_CORE_PROGRAM]->use();

    // Activate texture
    this->textures[TEX_PUSHEEN0]->bind(0);
    this->textures[TEX_CRATE1]->bind(1);

    // Draw
    this->meshes[MESH_QUAD]->render(this->shaders[SHADER_CORE_PROGRAM]);

    // Cleanup
    glfwSwapBuffers(window);
    glFlush();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Getters
int Game::getWindowShouldClose() {
    return glfwWindowShouldClose(this->window);
}

void Game::setWindowShouldClose() {
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

// Setters

// Static functions
// Callback method when resizing window
void Game::framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}


void Game::updateInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Game::updateInput(GLFWwindow* window, Mesh &mesh) {
    // Position 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //position.z += 0.001f; 
        mesh.move(glm::vec3(0.0f, 0.0f, 0.001f));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //position.x += 0.001f; 
        mesh.move(glm::vec3(0.001f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //position.z -= 0.001f; 
        mesh.move(glm::vec3(0.0f, 0.0f, -0.001f));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //position.x -= 0.001f; 
        mesh.move(glm::vec3(-0.001f, 0.0f, 0.0f));
    }

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        //rotation.y -= 0.1f; 
        mesh.rotate(glm::vec3(0.0f, -0.1f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        //rotation.y += 0.1f; 
        mesh.rotate(glm::vec3(0.0f, 0.1f, 0.0f));
    }

    // Scale
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        //scale += 0.01f; 
        mesh.scaleMesh(glm::vec3(0.01f));
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        //scale -= 0.01f;
        mesh.scaleMesh(glm::vec3(-0.01f));
    }
}
