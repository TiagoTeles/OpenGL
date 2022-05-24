#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GFX/Compute.h"
#include "GFX/Shader.h"
#include "GFX/SSBO.h"
#include "GFX/Texture.h"
#include "GFX/VAO.h"
#include "GFX/VBO.h"
#include "GFX/Window.h"

#define UNUSED(X) (void)(X)

float orthographic[] = {
    0.5625f, 0.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f, 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f,
};

float vertices[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
};

int main(int argc, char** argv) {
    UNUSED(argc);
    UNUSED(argv);

    // Window
    Window window(1600, 900, "OpenGL");

    // Import shaders
    Shader shader("res/default.vert", "res/default.frag");
    Compute compute("res/diffuse.comp");

    // Create test data
    float* data = new float[512 * 512]();

    // Create SSBO
    SSBO ssbo(0, data, 512 * 512 * sizeof(float));

    // Create buffers
    VBO vbo(vertices, 24 * sizeof(float));
    VAO vao;

    // Vertex array attributes
    vao.Bind();
    vbo.Bind();

    vao.setAttribute(vbo, 0, 2, 4 * sizeof(float), (void*) (0 * sizeof(float)));
    vao.setAttribute(vbo, 1, 2, 4 * sizeof(float), (void*) (2 * sizeof(float)));

    // Run compute shader
    compute.Bind();
    compute.setUniform1i("size_x", 512);
    compute.setUniform1i("size_y", 512);
    compute.Dispatch(16, 16, 1);
    
    // Run main loop
    while(!glfwWindowShouldClose(window.window)) {

        // Get input events
        glfwPollEvents();
        window.UpdateKeyboard();
        window.UpdateMouse();
        window.UpdateTitle();

        // Bind shader and VAO
        shader.Bind();
        vao.Bind();
        
        // Set Uniforms
        shader.setUniformMatrix4f("P", orthographic);
        shader.setUniform1i("size_x", 512);
        shader.setUniform1i("size_y", 512);

        // Draw to screen
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window.window);
    }

    // Delete Buffers
    vao.Delete();
    vbo.Delete();
    ssbo.Delete();

    // Delete Shders
    shader.Delete();
    compute.Delete();

    // Delete Window
    window.Delete();

    return EXIT_SUCCESS;
}
