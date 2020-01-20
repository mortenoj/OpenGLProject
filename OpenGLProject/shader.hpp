#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
private:
    // Private members
    GLuint id;
    const int versionMajor;
    const int versionMinor;

    // Private functions
    std::string loadShaderSource(const char* fileName);
    GLuint loadShader(GLenum type, const char* fileName);
    void linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader);

public:
    // Constructors/Destructor
    Shader(
            const int versionMajor,
            const int versionMinor,
            const char* vertexFile,
            const char* fragmentFile,
            const char* geometryFile = "");

    ~Shader();

    // Set uniform functions
    void use();
    void unuse();
    void setInt(GLint value, const GLchar* name);
    void setFloat(GLfloat value, const GLchar* name);
    void setVec2f(glm::fvec2 value, const GLchar* name);
    void setVec3f(glm::fvec3 value, const GLchar* name);
    void setVec4f(glm::fvec4 value, const GLchar* name);
    void setMat3f(glm::fmat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);
    void setMat4f(glm::fmat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);
};
