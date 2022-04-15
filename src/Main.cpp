#include <stdio.h>
#include <stdlib.h>

#include "EBO.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "Window.h"

#define UNUSED(X) (void)(X)

float screenV[] = {
    -1.0f, -1.0f, 0.5f, 0.5f, 0.5f,
     1.0f, -1.0f, 0.5f, 0.5f, 0.5f,
     1.0f,  1.0f, 0.5f, 0.5f, 0.5f,
    -1.0f,  1.0f, 0.5f, 0.5f, 0.5f,
};

GLuint screenI[] = {
    0, 1, 2,
    2, 3, 0,
};

float orthographic[] = {
    0.5625f, 0.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f, 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f,
};

int main(int argc, char** argv) {
    UNUSED(argc);
    UNUSED(argv);

    // Window
    Window window(1600, 900, "OpenGL");

    // Import shaders
    Shader shader("res/default.vert", "res/default.frag");

    // Create buffers
    VBO vbo(screenV, sizeof(screenV));
    EBO ebo(screenI, sizeof(screenI));
    VAO vao;

    // Vertex array attributes
    vao.Bind();
    vbo.Bind();
    ebo.Bind();

    vao.setAttribute(vbo, 0, 2, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    vao.setAttribute(vbo, 1, 3, 5 * sizeof(float), (void*)(2 * sizeof(float)));

    // Run main loop
    while(!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();

        // Draw to screen
        shader.Bind();
        vao.Bind();

        shader.setUniformMatrix4f("P", orthographic);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, sizeof(screenI)/sizeof(screenI[0]), GL_UNSIGNED_INT, (const void*) 0);

        glfwSwapBuffers(window.window);
    }

    printf("Hello World!\n");
    return EXIT_SUCCESS;
}