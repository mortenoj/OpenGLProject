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

const int WINDOW_WIDTH = 640*2;
const int WINDOW_HEIGHT = 480*2;

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
};

Vertex vertices[] = {
    // Position                      // Color                     // TexCoord
    { glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) }
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
        rotation.z -= 0.1f; 
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        rotation.z += 0.1f; 
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
    GLuint coreProgram = LoadShaders("res/shaders/core.vert", "res/shaders/core.frag");


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

    // Bind VAO 0
    glBindVertexArray(0);

    // Texture init

    // Texture 0
    int imageWidth = 0;
    int imageHeight = 0;

    unsigned char* image = SOIL_load_image("res/images/cat.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGBA);

    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (!image) {
        fprintf(stderr, "MAIN: Failed to load image\n");
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Properly destroy references
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    // Texture 1
    int imageWidth1 = 0;
    int imageHeight1 = 0;

    unsigned char* image1 = SOIL_load_image("res/images/crate.png", &imageWidth1, &imageHeight1, 0, SOIL_LOAD_RGBA);

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (!image1) {
        fprintf(stderr, "MAIN: Failed to load image1\n");
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth1, imageHeight1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Properly destroy references
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image1);

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

    // Init uniforms
    glUseProgram(coreProgram);
    glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

    glUseProgram(0);

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

        // Use a program
        glUseProgram(coreProgram);

        // Update uniforms
        glUniform1i(glGetUniformLocation(coreProgram, "texture0"), 0);
        glUniform1i(glGetUniformLocation(coreProgram, "texture1"), 1);

        // Move, rotate, scale
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, position);
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        ModelMatrix = glm::scale(ModelMatrix, scale);

        glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

        // Update projection matrix in case of resizing of window (should be done only when window is resizing)
        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

        ProjectionMatrix = glm::mat4(1.0f);
        ProjectionMatrix = glm::perspective(
            glm::radians(fov),
            static_cast<float>(frameBufferWidth) / frameBufferHeight,
            nearPlane,
            farPlane
        );

        glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

        // Activate texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
    
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

    // Delete shader program
    glDeleteProgram(coreProgram);


    // Delete VAO and buffers

    return EXIT_SUCCESS;
}
