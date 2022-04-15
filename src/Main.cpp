#include <stdio.h>
#include <stdlib.h>

#include "EBO.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "Window.h"

#define UNUSED(X) (void)(X)

#define N 256
#define ITER 10

#define INDEX(X, Y) ((Y) * N + (X))
#define SWAP(A, B) {float* C = A; A = B; B = C;}

float orthographic[] = {
    0.5625f, 0.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f, 0.0f,
       0.0f, 0.0f, 1.0f, 0.0f,
       0.0f, 0.0f, 0.0f, 1.0f,
};

int main(int argc, char** argv) {
    UNUSED(argc);
    UNUSED(argv);

    // Physical Quantities
    float diff = 1E-6;      // Diffusivity, [m^2/s]
    float size = 0.01;      // Length, [m]
    float velocity = 0.01;  // Velocity, [m/s]

    // Discretizations
    float dx = size / N;
    float dy = size / N;
    float dt = 0.01f;

    // Allocate memory
    float* Vx = new float[N*N]();
    float* Vy = new float[N*N]();
    float* rho = new float[N*N]();

    float* Vx0 = new float[N*N]();
    float* Vy0 = new float[N*N]();
    float* rho0 = new float[N*N]();

    // Draw Buffers
    float* vertices = new float[4*5 * N*N]();
    GLuint* indices = new GLuint[6 * N*N]();

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {

            // Setup velocity field
            Vx0[INDEX(x, y)] = velocity;
            Vy0[INDEX(x, y)] = velocity;

            // Add dye
            float radX = (2*((float) x/N) - 1.0f);
            float radY = (2*((float) y/N) - 1.0f);

            if (radX*radX + radY*radY < 0.01) {
                rho0[INDEX(x, y)] = 100.0f;
            }
        }
    }

    // Window
    Window window(1600, 900, "OpenGL");

    // Import shaders
    Shader shader("res/default.vert", "res/default.frag");

    // Run main loop
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window.window)) {

        // Get input events
        glfwPollEvents();

        // Run simulation
        float minDensity = NULL;
        float maxDensity = NULL;

        for (int i = 0; i < ITER; i++) {
            for (int y = 1; y < N-1; y++) {
                for (int x = 1; x < N-1; x++) {
                    
                    // Convective terms
                    float convX = -Vx0[INDEX(x, y)] * (rho[INDEX(x+1, y)] - rho[INDEX(x-1, y)]) / (2*dx);
                    float convY = -Vy0[INDEX(x, y)] * (rho[INDEX(x, y+1)] - rho[INDEX(x, y-1)]) / (2*dy);

                    // Diffusive terms
                    float diffX = (diff / (dx*dx)) * (rho[INDEX(x+1, y)] + rho[INDEX(x-1, y)]);
                    float diffY = (diff / (dy*dy)) * (rho[INDEX(x, y+1)] + rho[INDEX(x, y-1)]);

                    // Scaling factor
                    float c = 1 + (2*diff*dt/(dx*dx)) + (2*diff*dt/(dy*dy));

                    // Update density
                    rho[INDEX(x, y)] = (1/c) * (rho0[INDEX(x, y)] + dt * (convX + convY + diffX + diffY));

                    // Check solution bounds
                    if (rho[INDEX(x, y)] > maxDensity || maxDensity == NULL) {
                        maxDensity = rho[INDEX(x, y)];
                    }

                    if (rho[INDEX(x, y)] < minDensity || minDensity == NULL) {
                        minDensity = rho[INDEX(x, y)];
                    }
                }
            }
        }

        printf("Max Density: %f\n", maxDensity);
        printf("Min Density: %f\n", minDensity);

        // Draw density field
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                float x0 = (2.0f/N) * x - 1.0f;
                float y0 = (2.0f/N) * y - 1.0f; 
                float x1 = (2.0f/N) * (x+1) - 1.0f;
                float y1 = (2.0f/N) * (y+1) - 1.0f;

                vertices[INDEX(x, y)*20 + 0*5 + 0] = x0;
                vertices[INDEX(x, y)*20 + 0*5 + 1] = y0;
                vertices[INDEX(x, y)*20 + 0*5 + 2] = rho[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 0*5 + 3] = rho[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 0*5 + 4] = rho[INDEX(x, y)] / 100.0f;

                vertices[INDEX(x, y)*20 + 1*5 + 0] = x1;
                vertices[INDEX(x, y)*20 + 1*5 + 1] = y0;
                vertices[INDEX(x, y)*20 + 1*5 + 2] = rho[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 1*5 + 3] = rho[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 1*5 + 4] = rho[INDEX(x, y)] / 100.0f;

                vertices[INDEX(x, y)*20 + 2*5 + 0] = x1;
                vertices[INDEX(x, y)*20 + 2*5 + 1] = y1;
                vertices[INDEX(x, y)*20 + 2*5 + 2] = rho[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 2*5 + 3] = rho[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 2*5 + 4] = rho[INDEX(x, y)] / 100.0f;

                vertices[INDEX(x, y)*20 + 3*5 + 0] = x0;
                vertices[INDEX(x, y)*20 + 3*5 + 1] = y1;
                vertices[INDEX(x, y)*20 + 3*5 + 2] = rho[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 3*5 + 3] = rho[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 3*5 + 4] = rho[INDEX(x, y)] / 100.0f;

                indices[INDEX(x, y)*6 + 0] = INDEX(x, y)*4 + 0;
                indices[INDEX(x, y)*6 + 1] = INDEX(x, y)*4 + 1;
                indices[INDEX(x, y)*6 + 2] = INDEX(x, y)*4 + 2;
                indices[INDEX(x, y)*6 + 3] = INDEX(x, y)*4 + 2;
                indices[INDEX(x, y)*6 + 4] = INDEX(x, y)*4 + 3;
                indices[INDEX(x, y)*6 + 5] = INDEX(x, y)*4 + 0;
            }
        }

        // Swap buffers
        SWAP(rho, rho0);

        // Create buffers
        VBO vbo(vertices, 4 * 5 * N*N * sizeof(float));
        EBO ebo(indices, 6 * N*N * sizeof(GLuint));
        VAO vao;

        // Vertex array attributes
        vao.Bind();
        vbo.Bind();
        ebo.Bind();

        vao.setAttribute(vbo, 0, 2, 5 * sizeof(float), (void*)(0 * sizeof(float)));
        vao.setAttribute(vbo, 1, 3, 5 * sizeof(float), (void*)(2 * sizeof(float)));

        // Draw to screen
        shader.Bind();
        vao.Bind();

        // Set projection matrix
        shader.setUniformMatrix4f("P", orthographic);
        
        // Draw
        glDrawElements(GL_TRIANGLES, 6*N*N, GL_UNSIGNED_INT, (const void*) 0);
        glfwSwapBuffers(window.window);

        // Print FPS
        printf("FPS: %.2f\n\n", 1.0f / (glfwGetTime() - lastTime));
        lastTime = glfwGetTime();
    }

    return EXIT_SUCCESS;
}
