#include "game.hpp"

// Constructors/Destructor
Game::Game(
            const char* title,
            const int width, const int height,
            const int glMajVer, const int glMinVer,
            bool resizable
        ) :
            WINDOW_WIDTH(width),
            WINDOW_HEIGHT(height),
            GL_VERSION_MAJOR(glMajVer),
            GL_VERSION_MINOR(glMinVer),
            camera(
                    glm::vec3(0.0f, 0.0f, 1.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)
                ) {

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

    this->deltaTime = 0.0f;
    this->currentTime = 0.0f;
    this->lastTime = 0.0f;

    this->lastMouseX = 0.0;
    this->lastMouseY = 0.0;
    this->mouseX = 0.0;
    this->mouseY = 0.0;
    this->mouseOffsetX = 0.0;
    this->mouseOffsetY = 0.0;
    this->firstMouse = true;

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

    // Inputs
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    this->textures.push_back(new Texture("res/images/cat_specular.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("res/images/crate.png", GL_TEXTURE_2D));
    this->textures.push_back(new Texture("res/images/crate_specular.png", GL_TEXTURE_2D));
}

void Game::initMaterials() {
    this->materials.push_back(
            new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 0, 1)
        );
}

void Game::initMeshes() {
    Pyramid mesh = Pyramid();

    this->meshes.push_back(
            new Mesh(&mesh,
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
}

void Game::updateUniforms() {
    // Upate view matrix
    this->viewMatrix = this->camera.getViewMatrix();
    this->shaders[SHADER_CORE_PROGRAM]->setMat4f(this->viewMatrix, "ViewMatrix");
    this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

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
void Game::updateDeltaTime() {
    this->currentTime = static_cast<float>(glfwGetTime());
    this->deltaTime = this->currentTime - this->lastTime;
    this->lastTime = this->currentTime;
}

void Game::updateKeyboardInput() {
    // Close window on ESC
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        this->setWindowShouldClose();
    }

    // Camera
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
        this->camera.move(this->deltaTime, FORWARD);
    }
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
        this->camera.move(this->deltaTime, BACKWARD);
    }
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
        this->camera.move(this->deltaTime, LEFT);
    }
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
        this->camera.move(this->deltaTime, RIGHT);
    }

    if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->cameraPosition.y += 0.01f;
    }
    if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS) {
        this->cameraPosition.y -= 0.01f;
    }
}

void Game::updateMouseInput() {
    glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse) {
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	//Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	//Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void Game::updateInput() {
    // Listen for inputs
    glfwPollEvents();

    // Update inputs
    this->updateKeyboardInput();
    this->updateMouseInput();

    this->camera.updateInput(this->deltaTime, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::update() {
    this->updateDeltaTime();
    this->updateInput();

    this->meshes[0]->rotate(glm::vec3(0.0f, 0.01f, 0.0f));
}

// Render function
void Game::render() {
    //  Clear
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Update uniforms
    this->updateUniforms();

    // Update uniforms
    this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);

    // Use a program
    this->shaders[SHADER_CORE_PROGRAM]->use();

    // Activate texture
    this->textures[TEX_CRATE]->bind(0);
    this->textures[TEX_CRATE_SPECULAR]->bind(1);

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
