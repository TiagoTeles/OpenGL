#include "VAO.h"
#include "VBO.h"

#include <glad/glad.h>

VAO::VAO() {
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);
}

void VAO::Bind() {
    glBindVertexArray(ID);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}

void VAO::setAttribute(VBO& vbo, GLuint index, GLint size, GLsizei stride, const void* offset) {
    vbo.Bind();
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(index);
}