#include "Compute.h"

#include "stdio.h"
#include "stdlib.h"

#include <fstream>
#include <string>

#include <glad/glad.h>

static std::string readFile(const char* path) {

    // Open file
    std::ifstream file(path, std::ios::binary);

    if (file.fail()) {
        printf("Failed to open file!\n");
        exit(EXIT_FAILURE);
    }

    // Get file length
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file
    std::string shader(length, '\0');
    file.read(&shader[0], length);

    // Close file
    file.close();

    return shader;
}

static void logError(GLuint ID) {
    int length;

    // Get error length
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);

    // Read error
    std::string error(length, '\0');
    glGetShaderInfoLog(ID, length, &length, &error[0]);

    // Print error
    printf("%s", error.c_str());
}


Compute::Compute(const char* compPath) {
    GLint compStatus;
    GLint linkStatus;

    // Import shader
    std::string compStr = readFile(compPath);
    const char* compCStr = compStr.c_str();

    // Compile shader
    GLuint compShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compShader, 1, &compCStr, NULL);
    glCompileShader(compShader);

    // Link shader
    ID = glCreateProgram();
    glAttachShader(ID, compShader);
    glLinkProgram(ID);

    // Check for errors
    glGetShaderiv(compShader, GL_COMPILE_STATUS, &compStatus);
    glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);

    if (compShader == GL_FALSE) {
        logError(compShader);
    }

    if (linkStatus == GL_FALSE) {
        logError(ID);
    }

    // Cleanup
    glDeleteShader(compShader);
}

void Compute::Bind() {
    glUseProgram(ID);
}

void Compute::Unbind() {
    glUseProgram(0);
}

void Compute::Delete() {
    glDeleteProgram(ID);
}

void Compute::Dispatch(GLuint x, GLuint y, GLuint z) {
    glUseProgram(ID);
    glDispatchCompute(x, y, z);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void Compute::setUniform1i(const char* name, int i) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform1i(location, i);
}

void Compute::setUniform2i(const char* name, int i, int j) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform2i(location, i, j);
}

void Compute::setUniform3i(const char* name, int i, int j, int k) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform3i(location, i, j, k);
}

void Compute::setUniform4i(const char* name, int i, int j, int k, int l) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform4i(location, i, j, k, l);
}

void Compute::setUniform1f(const char* name, float x) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform1f(location, x);
}

void Compute::setUniform2f(const char* name, float x, float y) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform2f(location, x, y);
}

void Compute::setUniform3f(const char* name, float x, float y, float z) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform3f(location, x, y, z);
}

void Compute::setUniform4f(const char* name, float x, float y, float z, float w) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform4f(location, x, y, z, w);
}

void Compute::setUniformMatrix4f(const char* name, float* matrix) {
    GLint location = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, matrix);
}