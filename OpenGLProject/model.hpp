#pragma once

#include "mesh.hpp"
#include "texture.hpp"
#include "material.hpp"

class Model {
private:
    // Private members
    Material* material;
    Texture* overrideTextureDiffuse;
    Texture* overrideTextureSpecular;
    std::vector<Mesh*> meshes;
    glm::vec3 position;

    // Private functions
    void updateUniforms();
public:
    // Constructors
    Model();
    Model(
            glm::vec3 position,
            Material* material,
            Texture* overrideTexturexDiffuse,
            Texture* overrideTexturexSpecular,
            std::vector<Mesh*> meshes
        );
    ~Model();

    // Public functions
    void move(const glm::vec3 value);
    void scaleModel(const glm::vec3 value);
    void rotate(const glm::vec3 rotation);
    void update();
    void render(Shader* shader);
};
