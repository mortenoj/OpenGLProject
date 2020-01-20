#include "primitives.hpp"

// ================================ Primitive parent class ================================

Primitive::Primitive() {}
Primitive::~Primitive() {}

void Primitive::setVertexData(
        const Vertex* vertices, const unsigned nrOfVertices,
        const GLuint* indices, const unsigned nrOfIndices) {
    // Empty out the vectors before adding
    this->vertices.clear();
    this->indices.clear();
    this->addVertexData(vertices, nrOfVertices, indices, nrOfIndices);
}

void Primitive::addVertexData(
        const Vertex* vertices, const unsigned nrOfVertices,
        const GLuint* indices, const unsigned nrOfIndices) {
    for (size_t i = 0; i < nrOfVertices; i++) {
        this->vertices.push_back(vertices[i]);
    }

    for (size_t i = 0; i < nrOfIndices; i++) {
        this->indices.push_back(indices[i]);
    }
}

// Getters
Vertex* Primitive::getVertices() { return this->vertices.data(); }
GLuint* Primitive::getIndices() { return this->indices.data(); }
const size_t Primitive::getNrOfVertices() { return this->vertices.size(); }
const size_t Primitive::getNrOfIndices() { return this->indices.size(); }

// ================================ Triangle class ================================

Triangle::Triangle() : Primitive() {
    Vertex vertices[] = {
        // Position                      // Color                     // TexCoord            // Normal
        { glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) }
        //{ glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }
    };

    unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

    GLuint indices[] = {
        0, 1, 2
    };

    unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

    this->setVertexData(vertices, nrOfVertices, indices, nrOfIndices);
}

// ================================ Quad class ================================

Quad::Quad() : Primitive() {
    Vertex vertices[] = {
        // Position                      // Color                     // TexCoord            // Normal
        { glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }
    };

    unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

    GLuint indices[] = {
        0, 1, 2, // Triangle 1
        0, 2, 3  // Triangle 2
    };

    unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

    this->setVertexData(vertices, nrOfVertices, indices, nrOfIndices);
}