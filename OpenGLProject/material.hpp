#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.hpp"

class Material {
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLint diffuseTex;
    GLint specularTex;

public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex);
    ~Material();

    void sendToShader(Shader &program);
};
