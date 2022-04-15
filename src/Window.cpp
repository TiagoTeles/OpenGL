#include "window.h"

#include "stdio.h"
#include "stdlib.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void errorCallback(int, const char* description) {
    printf(description);
    exit(EXIT_FAILURE);
}

void APIENTRY debugCallback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar* message, const void*) {
    printf(message);
    exit(EXIT_FAILURE);
}

Window::Window(int width, int height, const char* name) {

    // Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW initialization failed!");
        exit(EXIT_FAILURE);
    }

    // Setup GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create a windowed mode window and its OpenGL context`
    window = glfwCreateWindow(width, height, name, NULL, NULL);

    if (!window) {
        printf("GLFW window creation failed!");
        exit(EXIT_FAILURE);
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGL()) {
        printf("GLAD initialization failed!");
        exit(EXIT_FAILURE);
    }

    // Enable VSync
    glfwSwapInterval(1);

    // Error Callbacks
    glfwSetErrorCallback(errorCallback);
    glDebugMessageCallback(debugCallback, NULL);
}

void Window::Delete() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::UpdateKeyboard() {
    for (int i = 32; i < GLFW_KEY_LAST; i++) {
        keyboard.keys[i] = glfwGetKey(window, i);
    }
}

void Window::UpdateMouse() {
    double xpos;
    double ypos;

    glfwGetCursorPos(window, &xpos, &ypos);
    mouse.dx = xpos - mouse.x;
    mouse.dy = ypos - mouse.y;
    mouse.x = xpos;
    mouse.y = ypos;

    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
        mouse.buttons[i] = glfwGetMouseButton(window, i);
    }
}
