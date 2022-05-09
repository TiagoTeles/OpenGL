#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>


class Texture {
   private: 
    GLuint ID;

   public:
    GLuint slot;

   public:
    Texture(GLuint slot, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data);

    void Bind();
    void Unbind();
    void Delete();
};

#endif