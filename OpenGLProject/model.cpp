#include "model.hpp"

// Constructors
Model::Model() {}

Model::Model(
            glm::vec3 position,
            Material* material,
            Texture* overrideTextureDiffuse,
            Texture* overrideTextureSpecular,
            std::vector<Mesh*> meshes
        ) {
    this->position = position;
    this->material = material;
    this->overrideTextureDiffuse = overrideTextureDiffuse;
    this->overrideTextureSpecular = overrideTextureSpecular;

    for (auto *mesh : meshes) {
        this->meshes.push_back(new Mesh(*mesh));
    }

    for (auto &mesh : this->meshes) {
        mesh->move(this->position);
        mesh-> setOrigin(this->position);
    }
}

Model::~Model() {
    for (auto &elem : meshes) {
        delete elem;
    }
}

// Private functions
void Model::updateUniforms() {

}

// Public functions
void move(const glm::vec3 value) {

}

void scaleModel(const glm::vec3 value) {

}

void Model::rotate(const glm::vec3 rotation) {
    for (auto &elem : this->meshes) {
        elem->rotate(rotation);
    }
}

void Model::update() {

}

void Model::render(Shader* shader) {
    // Update uniforms
    this->updateUniforms();

    // Update uniforms
    this->material->sendToShader(*shader);

    // Use the program
    shader->use();

    // Activate texture
    this->overrideTextureDiffuse->bind(0);
    this->overrideTextureSpecular->bind(1);

    // Draw
    for (auto &elem : meshes) {
        elem->render(shader);
    }
}
