#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"


// Constructors/Destructor
Shader::Shader(
        const int versionMajor,
        const int versionMinor,
        const char* vertexFile,
        const char* fragmentFile,
        const char* geometryFile) : versionMajor(versionMajor), versionMinor(versionMinor) {
    GLuint vertexShader = 0;
    GLuint geometryShader = 0;
    GLuint fragmentShader = 0;

    vertexShader = this->loadShader(GL_VERTEX_SHADER, vertexFile);
    fragmentShader = this->loadShader(GL_FRAGMENT_SHADER, fragmentFile);
    if (std::strlen(geometryFile) != 0)
        geometryShader = this->loadShader(GL_GEOMETRY_SHADER, geometryFile);

    this->linkProgram(vertexShader, fragmentShader, geometryShader);

    // Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
    if (geometryShader)
        glDeleteShader(geometryShader);
}

Shader::~Shader() {
    glDeleteProgram(this->id);
}

std::string Shader::loadShaderSource(const char* fileName) {
	std::string shaderCode;
	std::ifstream shaderStream(fileName, std::ios::in);
	if (shaderStream.is_open()) {
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	} else {
		printf("Impossible to open %s.\n", fileName);
		getchar();
		return 0;
	}

    std::string versionNr =
        std::to_string(this->versionMajor) +
        std::to_string(this->versionMinor) + 
        "0";

    shaderCode.replace(shaderCode.find("#version"), 12, "#version " + versionNr);

    return shaderCode;
}

// Private functions

GLuint Shader::loadShader(GLenum type, const char* fileName) {
	GLuint shaderID = glCreateShader(type);

	GLint result = GL_FALSE;
	int infoLogLen;

	printf("Compiling shader : %s\n", fileName);
    const std::string str_src = this->loadShaderSource(fileName);
	const char* shaderSource = str_src.c_str();
	glShaderSource(shaderID, 1, &shaderSource , NULL);
	glCompileShader(shaderID);

	// Check Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLen);
	if ( infoLogLen > 0 ){
		std::vector<char> shaderErrorMsg(infoLogLen+1);
		glGetShaderInfoLog(shaderID, infoLogLen, NULL, &shaderErrorMsg[0]);
		printf("%s\n", &shaderErrorMsg[0]);
	}

    return shaderID;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader) {
	GLint result = GL_FALSE;
	int infoLogLen;

	printf("Linking program\n");

	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShader);
	glAttachShader(this->id, fragmentShader);

    // GeometryShader is optional
    if (geometryShader)
        glAttachShader(this->id, geometryShader);

	glLinkProgram(this->id);

	// Check the program
	glGetProgramiv(this->id, GL_LINK_STATUS, &result);
	glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &infoLogLen);
	if ( infoLogLen > 0 ){
		std::vector<char> programErrorMsg(infoLogLen+1);
		glGetProgramInfoLog(this->id, infoLogLen, NULL, &programErrorMsg[0]);
		printf("%s\n", &programErrorMsg[0]);
	}
	
	glDetachShader(this->id, vertexShader);
	glDetachShader(this->id, fragmentShader);
    if (geometryShader)
        glDetachShader(this->id, geometryShader);

    glUseProgram(0);
}

// Uniform functions (public)

void Shader::use() {
    glUseProgram(this->id);
}

void Shader::unuse() {
    glUseProgram(0);
}

void Shader::setInt(GLint value, const GLchar* name) {
    this->use();
    glUniform1i(glGetUniformLocation(this->id, name), value);
    this->unuse();
}

void Shader::setFloat(GLfloat value, const GLchar* name) {
    this->use();
    glUniform1f(glGetUniformLocation(this->id, name), value);
    this->unuse();
}

void Shader::setVec2f(glm::fvec2 value, const GLchar* name) {
    this->use();
    glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
    this->unuse();
}

void Shader::setVec3f(glm::fvec3 value, const GLchar* name) {
    this->use();
    glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
    this->unuse();
}

void Shader::setVec4f(glm::fvec4 value, const GLchar* name) {
    this->use();
    glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
    this->unuse();
}

void Shader::setMat3f(glm::fmat3 value, const GLchar* name, GLboolean transpose) {
    this->use();
    glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
    this->unuse();
}

void Shader::setMat4f(glm::fmat4 value, const GLchar* name, GLboolean transpose) {
    this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
    this->unuse();
}

