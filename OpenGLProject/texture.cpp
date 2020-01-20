#include "texture.hpp"

Texture::Texture(const char* fileName, GLenum type) {
    this->type = type;

    unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, 0, SOIL_LOAD_RGBA);

    glGenTextures(1, &this->id);
    glBindTexture(type, this->id);

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (!image) {
        fprintf(stderr, "TEXTURE: Failed to load image %s\n", fileName);
    } else {
        glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(type);
    }

    // Properly destroy references
    glActiveTexture(0);
    glBindTexture(type, 0);
    SOIL_free_image_data(image);
}

void Texture::loadFromFile(const char* fileName) {
    if (this->id) {
        glDeleteTextures(1, &this->id);
    }

    unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, 0, SOIL_LOAD_RGBA);

    glGenTextures(1, &this->id);
    glBindTexture(this->type, this->id);

    glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (!image) {
        fprintf(stderr, "loadFromFile: Failed to load image %s\n", fileName);
    } else {
        glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(this->type);
    }

    // Properly destroy references
    glActiveTexture(0);
    glBindTexture(this->type, 0);
    SOIL_free_image_data(image);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->id);
}

void Texture::bind(const GLint textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(this->type, this->id);
}

void Texture::unbind() {
    glActiveTexture(0);
    glBindTexture(this->type, 0);
}
