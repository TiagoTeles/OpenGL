#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>

#include "VBO.h"

class VAO {
   private:
    GLuint ID;

   public:
    VAO();

    void Bind();
    void Unbind();
    void Delete();

    void setAttribute(VBO& vbo, GLuint index, GLint size, GLsizei stride, const void* offset);
};

#endif