#include "mesh.hpp"

// Constructors/Destructor

Mesh::Mesh() {}
Mesh::Mesh(
        Primitive* primitive,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale) {

    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    // This order matters
    this->initVAO(primitive);
    this->updateModelMatrix();
}

Mesh::Mesh(
        Vertex* vertexArray,
        const unsigned &nrOfVertices,
        GLuint* indexArray,
        const unsigned &nrOfIndices,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale) {

    this->position = position;
    this->rotation = rotation;
    this->scale = scale;

    // This order matters
    this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
    this->updateModelMatrix();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

// Private functions
void Mesh::initVAO(Primitive* primitive) {
    this->nrOfVertices = primitive->getNrOfVertices();
    this->nrOfIndices = primitive->getNrOfIndices();

    // Gen VAO and bind
    glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

    // Gen VBO, bind and send data
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);
    
    // Gen EBO, bind and send data
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

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
}

void Mesh::initVAO(
            Vertex* vertexArray,
            const unsigned &nrOfVertices,
            GLuint* indexArray,
            const unsigned &nrOfIndices) {
    // Set variables
    this->nrOfVertices = nrOfVertices;
    this->nrOfIndices = nrOfIndices;

    // Gen VAO and bind
    glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

    // Gen VBO, bind and send data
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);
    
    // Gen EBO, bind and send data
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

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
}

void Mesh::updateUniforms(Shader* shader) {
    shader->setMat4f(this->modelMatrix, "ModelMatrix");
}

void Mesh::updateModelMatrix() {
    this->modelMatrix = glm::mat4(1.0f);
    this->modelMatrix = glm::translate(this->modelMatrix, this->position);
    this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    this->modelMatrix = glm::scale(this->modelMatrix, this->scale);
}

// Public functions

// Upate function
void Mesh::update() {}

// Render function
void Mesh::render(Shader* shader) {
    // Update uniforms
    this->updateModelMatrix();
    this->updateUniforms(shader);

    shader->use();

    // Bind vertex array object
    glBindVertexArray(this->VAO);

    // Render
    if (this->nrOfIndices == 0) {
        glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
    } else {
        glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
    }
}

// Move adds to current position
void Mesh::move(const glm::vec3 value) { this->position += value; }
void Mesh::rotate(const glm::vec3 value) { this->rotation += value; }
void Mesh::scaleMesh(const glm::vec3 value) { this->scale += value; }

// Getters
glm::vec3 Mesh::getPosition() { return this->position; }
glm::vec3 Mesh::getRotation() { return this->rotation; }
glm::vec3 Mesh::getScale() { return this->scale; }

// Setters
void Mesh::setPosition(const glm::vec3 position) { this->position = position; }
void Mesh::setRotation(const glm::vec3 rotation) { this->rotation = rotation; }
void Mesh::setScale(const glm::vec3 scale) { this->scale = scale; }





