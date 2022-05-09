#ifndef COMPUTE_H
#define COMPUTE_H

#include <glad/glad.h>

class Compute {
   private:
    GLuint ID;

   public:
    Compute(const char* compPath);

    void Bind();
    void Unbind();
    void Delete();
    void Dispatch(GLuint x, GLuint y, GLuint z);
};

#endif
