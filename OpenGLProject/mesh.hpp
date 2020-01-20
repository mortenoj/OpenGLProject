#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "vertex.hpp"
#include "shader.hpp"
#include "primitives.hpp"
#include "texture.hpp"
#include "material.hpp"


class Mesh {
private:
    unsigned nrOfVertices;
    unsigned nrOfIndices;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 modelMatrix;
    
    void initVAO(
            Vertex* vertexArray,
            const unsigned &nrOfVertices,
            GLuint* indexArray,
            const unsigned &nrOfIndices
        );
    void initVAO(Primitive* primitive);

    void updateUniforms(Shader* shader);
    void updateModelMatrix();

public:
    Mesh(); // Empty constructor

    // Init mesh with primitive
    Mesh(
        Primitive* primitive,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale);

    // Init mesh with vertices and indices
    Mesh(
        Vertex* vertexArray, const unsigned &nrOfVertices,
        GLuint* indexArray, const unsigned &nrOfIndices,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f));

    // Destructor
    ~Mesh();

    // Public functions
    void update();
    void render(Shader* shader);

    void move(const glm::vec3 position);
    void rotate(const glm::vec3 value);
    void scaleMesh(const glm::vec3 value);

    // Getters
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getScale();

    // Setters
    void setPosition(const glm::vec3 position);
    void setRotation(const glm::vec3 rotation);
    void setScale(const glm::vec3 scale);
};
