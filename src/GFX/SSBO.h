#ifndef SSBO_H
#define SSBO_H

#include <glad/glad.h>

class SSBO {
   private:
    GLuint ID;
    GLuint index;

   public:
    SSBO(GLuint index, const void* data, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif