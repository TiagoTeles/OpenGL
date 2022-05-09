#include "texture.h"

#include <glad/glad.h>

Texture::Texture(GLuint slot, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data) {
    this->slot = slot;

    // Bind texture slot
    glActiveTexture(GL_TEXTURE0 + slot);

    // Create texture
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Configure texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Assign texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, data);
}

void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    glActiveTexture(GL_TEXTURE0 + slot);
    glDeleteTextures(1, &ID);
}