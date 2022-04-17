#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

class EBO {
   private:
    GLuint ID;
    
   public:
    EBO(const void* data, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif