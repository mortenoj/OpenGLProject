#pragma once

#include <GL/glew.h>
#include <vector>

#include "vertex.hpp"

// Parent class
class Primitive {
private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

public:
    Primitive();
    virtual ~Primitive();

    // Functions
    void setVertexData(
            const Vertex* vertices, const unsigned nrOfVertices,
            const GLuint* indices, const unsigned nrOfIndices);
    void addVertexData(
            const Vertex* vertices, const unsigned nrOfVertices,
            const GLuint* indices, const unsigned nrOfIndices);

    // Getters
    Vertex* getVertices();
    GLuint* getIndices();
    const size_t getNrOfVertices();
    const size_t getNrOfIndices();
};

// Class for handling triangle data
class Triangle : public Primitive {
public:
    Triangle();
};

// Class for handling quad data
class Quad : public Primitive {
public:
    Quad();
};

// Class for handling pyramids
class Pyramid : public Primitive {
public:
    Pyramid();
};

// Class for handling cubes
class Cube : public Primitive {
public:
    Cube();
};
