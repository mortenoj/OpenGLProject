#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "model.hpp"
#include "camera.hpp"

// ENUMS
enum ShaderEnum{ SHADER_CORE_PROGRAM = 0 };
enum TextureEnum{
    TEX_CAT = 0,
    TEX_CAT_SPECULAR = 1,
    TEX_CRATE = 2,
    TEX_CRATE_SPECULAR = 3
};
enum MaterialEnum{ MAT_1 = 0 };
enum MeshEnum{ MESH_QUAD = 0 };

class Game {
private:
    // Window
    GLFWwindow* window;
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    int frameBufferWidth;
    int frameBufferHeight;

    // OpenGL context
    const int GL_VERSION_MAJOR;
    const int GL_VERSION_MINOR;

    // Delta time
    float deltaTime;
    float currentTime;
    float lastTime;

    // Mouse input
    double lastMouseX;
    double lastMouseY;
    double mouseX;
    double mouseY;
    double mouseOffsetX;
    double mouseOffsetY;
    bool firstMouse;

    // Camera
    Camera camera;

    // Matrices
    glm::mat4 viewMatrix;
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 worldUp;

    glm::mat4 projectionMatrix;
    float fov;
    float zNear;
    float zFar;

    // Shaders
    std::vector<Shader*> shaders;

    // Textures
    std::vector<Texture*> textures;

    // Material
    std::vector<Material*> materials;

    // Models
    std::vector<Model*> models;

    // Lights
    std::vector<glm::vec3*> lights;

    // Static variables

    // Private functions
    void initGLFW();
    void initWindow(const char* title, bool resizable);
    void initGLEW(); // After context creation
    void initOpenGLOptions();
    void initMatrices();
    void initShaders();
    void initTextures();
    void initMaterials();
    void initModels();
    void initLights();
    void initUniforms();

    void updateUniforms();

public:
    // Constructors/Destructor
    Game(
            const char* title,
            const int width, const int height,
            const int GLMajorVersion, const int GLMinorVersion,
            bool resizable
        );

    virtual ~Game();

    // Public functions
    void updateDeltaTime();
    void updateKeyboardInput();
    void updateMouseInput();
    void updateInput();
    void update();
    void render();

    // Getters
    int getWindowShouldClose();

    // Setters
    void setWindowShouldClose();

    // Static functions
    static void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
};
