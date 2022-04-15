#include "Shader.h"

#include "stdio.h"
#include "stdlib.h"

#include <fstream>
#include <string>

#include <glad/glad.h>

std::string readFile(const char* path) {

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

void logError(GLuint ID) {
    int length;

    // Get error length
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);

    // Read error
    std::string error(length, '\0');
    glGetShaderInfoLog(ID, length, &length, &error[0]);

    // Print error
    printf("%s", error.c_str());
}


Shader::Shader(const char* vertPath, const char* fragPath) {
    GLint vertStatus;
    GLint fragStatus;
    GLint linkStatus;

    // Import shaders
    std::string vertStr = readFile(vertPath);
    std::string fragStr = readFile(fragPath);
    
    const char* test1 = vertStr.c_str();
    const char* test2 = fragStr.c_str();

    // Create shader objects
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile shaders
    glShaderSource(vertShader, 1, &test1, NULL);
    glShaderSource(fragShader, 1, &test2, NULL);
    glCompileShader(vertShader);
    glCompileShader(fragShader);

    // Link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);

    // Check for errors
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertStatus);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragStatus);
    glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);

    if (vertStatus == GL_FALSE) {
        logError(vertShader);
    }

    if (fragStatus == GL_FALSE) {
        logError(fragShader);
    }

    if (linkStatus == GL_FALSE) {
        logError(ID);
    }

    // Cleanup
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

void Shader::Bind() {
    glUseProgram(ID);
}

void Shader::Unbind() {
    glUseProgram(0);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}

void Shader::setUniform1i(const char* name, int i) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform1i(location, i);
}

void Shader::setUniform2i(const char* name, int i, int j) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform2i(location, i, j);
}

void Shader::setUniform3i(const char* name, int i, int j, int k) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform3i(location, i, j, k);
}

void Shader::setUniform4i(const char* name, int i, int j, int k, int l) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform4i(location, i, j, k, l);
}

void Shader::setUniform1f(const char* name, float x) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform1f(location, x);
}

void Shader::setUniform2f(const char* name, float x, float y) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform2f(location, x, y);
}

void Shader::setUniform3f(const char* name, float x, float y, float z) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform3f(location, x, y, z);
}

void Shader::setUniform4f(const char* name, float x, float y, float z, float w) {
    GLint location = glGetUniformLocation(ID, name);
    glUniform4f(location, x, y, z, w);
}

void Shader::setUniformMatrix4f(const char* name, float* matrix) {
    GLint location = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, matrix);
}
