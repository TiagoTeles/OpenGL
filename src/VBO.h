#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

class VBO {
   private:
    GLuint ID;

   public:
    VBO(const void* data, GLsizeiptr size);
    
    void Bind();
    void Unbind();
    void Delete();
};

#endif