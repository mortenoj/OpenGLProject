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

#include "shader.hpp"
#include "texture.hpp"

const int WINDOW_WIDTH = 640*2;
const int WINDOW_HEIGHT = 480*2;

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

Vertex vertices[] = {
    // Position                      // Color                     // TexCoord            // Normal
    { glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
    { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
    { glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
    { glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) }
};

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
    0, 1, 2, // Triangle 1
    0, 2, 3  // Triangle 2
};

unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}

void updateInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void updateInput(GLFWwindow* window, glm::vec3 &position, glm::vec3 &rotation, glm::vec3 &scale) {
    // Position 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position.z += 0.001f; 
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position.x += 0.001f; 
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position.z -= 0.001f; 
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position.x -= 0.001f; 
    }

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        rotation.y -= 0.1f; 
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        rotation.y += 0.1f; 
    }

    // Scale
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        scale += 0.01f; 
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        scale -= 0.01f;
    }
}

int main() {
    // Init glfw
    glfwInit();

    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learning OpenGL", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "MAIN: Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

    int frameBufferWidth = 0;
    int frameBufferHeight = 0;
    //glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwMakeContextCurrent(window); // Important for GLEW

    // Init GLEW (needs window and opengl context)
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "MAIN: Failed to initialize GLEW\n");
        glfwTerminate();
    }


    // OpenGL options
    glEnable(GL_DEPTH_TEST); // Enables Z coordinate

    glEnable(GL_CULL_FACE); // Removes unnecessary faces
    glCullFace(GL_BACK); // Cull backsides of triangles
    glFrontFace(GL_CCW); // Set OpenGL to render triangles counter clock-wise

    glEnable(GL_BLEND); // Blending of colors
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // For funs
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // WIREFRAME


    // Shader init
    Shader coreProgram("res/shaders/core.vert", "res/shaders/core.frag");

    // Model

    
    // VAO, VBO, EBO
    
    // Gen VAO and bind
    GLuint VAO;
    //glCreateVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    // Gen VBO, bind and send data
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Gen EBO, bind and send data
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers and enable (input assembly)
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // TexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    // Normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);

    // Bind VAO 0
    glBindVertexArray(0);

    // Texture init
    Texture texture0("res/images/cat.png", GL_TEXTURE_2D, 0);
    Texture texture1("res/images/crate.png", GL_TEXTURE_2D, 1);

    // Init matrices
    glm::vec3 position(0.0f);
    glm::vec3 rotation(0.0f);
    glm::vec3 scale(1.0f);

    // Model Matrix
    glm::mat4 ModelMatrix(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, position);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    ModelMatrix = glm::scale(ModelMatrix, scale);

    // View Matrix
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    glm::vec3 camFront(0.0f, 0.0f, -1.0f);
    glm::vec3 camPosition(0.0f, 0.0f, 1.0f);
    glm::mat4 ViewMatrix(1.0f);

    ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

    // Projection Matrix
    float fov = 90.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    glm::mat4 ProjectionMatrix(1.0f);

    ProjectionMatrix = glm::perspective(
            glm::radians(fov),
            static_cast<float>(frameBufferWidth) / frameBufferHeight,
            nearPlane,
            farPlane
        );

    // Lights
    glm::vec3 lightPos0(0.0f, 0.0f, 1.0f);

    // Init uniforms
    coreProgram.setMat4f(ModelMatrix, "ModelMatrix");
    coreProgram.setMat4f(ViewMatrix, "ViewMatrix");
    coreProgram.setMat4f(ProjectionMatrix, "ProjectionMatrix");

    coreProgram.setVec3f(lightPos0, "lightPos0");
    coreProgram.setVec3f(camPosition, "cameraPos");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Update input ---
        glfwPollEvents();
        updateInput(window, position, rotation, scale);

        // Update
        updateInput(window);
        
        // Draw ---
        //  Clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Update uniforms
        coreProgram.setInt(texture0.getTextureUnit(), "texture0");
        coreProgram.setInt(texture1.getTextureUnit(), "texture1");

        // Move, rotate, scale
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, position);
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        ModelMatrix = glm::scale(ModelMatrix, scale);

        coreProgram.setMat4f(ModelMatrix, "ModelMatrix");

        // Update projection matrix in case of resizing of window (should be done only when window is resizing)
        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

        ProjectionMatrix = glm::mat4(1.0f);
        ProjectionMatrix = glm::perspective(
            glm::radians(fov),
            static_cast<float>(frameBufferWidth) / frameBufferHeight,
            nearPlane,
            farPlane
        );

        coreProgram.setMat4f(ProjectionMatrix, "ProjectionMatrix");

        // Use a program
        coreProgram.use();

        // Activate texture
        texture0.bind();
        texture1.bind();
    
        // Bind vertex array object
        glBindVertexArray(VAO);

        // Draw
        //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

        // --- End draw
        glfwSwapBuffers(window);
        glFlush();

        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // End of program ---
    glfwDestroyWindow(window);
    glfwTerminate();

    // Delete VAO and buffers

    return EXIT_SUCCESS;
}
