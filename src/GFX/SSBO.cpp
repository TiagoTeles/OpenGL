#include "SSBO.h"

#include <glad/glad.h>

SSBO::SSBO(GLuint index, const void* data, GLsizeiptr size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);    
}

void SSBO::Bind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ID);
}

void SSBO::Unbind() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);
}

void SSBO::Delete() {
    glDeleteBuffers(1, &ID);
}
