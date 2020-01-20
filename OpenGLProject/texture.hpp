#pragma once

#include <iostream>
#include <GL/glew.h>
#include "SOIL2/SOIL2.h"

class Texture {
private:
    GLuint id;
    int width;
    int height;
    unsigned int type;

public:
    Texture(const char* fileName, GLenum type);
    ~Texture();

    void bind(const GLint textureUnit);
    void unbind();
    void loadFromFile(const char* fileName);

    // Getters
    inline GLuint getId() const { return this->id; }
    inline int getWidth() const { return this->width; }
    inline int getHeight() const { return this->height; }
};

