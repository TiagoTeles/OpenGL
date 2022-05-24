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

    void setUniform1i(const char* name, int i);
    void setUniform2i(const char* name, int i, int j);
    void setUniform3i(const char* name, int i, int j, int k);
    void setUniform4i(const char* name, int i, int j, int k, int l);

    void setUniform1f(const char* name, float x);
    void setUniform2f(const char* name, float x, float y);
    void setUniform3f(const char* name, float x, float y, float z);
    void setUniform4f(const char* name, float x, float y, float z, float w);

    void setUniformMatrix4f(const char* name, float* matrix);
};

#endif
