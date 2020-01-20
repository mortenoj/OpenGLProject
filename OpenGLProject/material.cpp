#include "material.hpp"

// Constructors/Destructor

Material::Material(
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        GLint diffuseTex,
        GLint specularTex) {

    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->diffuseTex = diffuseTex;
    this->specularTex = specularTex;
}

Material::~Material() {}

// Public functions

void Material::sendToShader(Shader &program) {
    program.setVec3f(this->ambient, "material.ambient");
    program.setVec3f(this->diffuse, "material.diffuse");
    program.setVec3f(this->specular, "material.specular");
    program.setInt(this->diffuseTex, "material.diffuseTex");
    program.setInt(this->specularTex, "material.specularTex");
}
