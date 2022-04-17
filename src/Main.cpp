#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "EBO.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "Window.h"

#define UNUSED(X) (void)(X)

#define N 128
#define ITER 16

#define INDEX(X, Y) ((Y) * N + (X))
#define SWAP(A, B) {float* C = A; A = B; B = C;}

// TODO: REDUCE NUMBER OF VERTICES

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
    float velocity = 0.01;  // Velocity, [m/s]
    float size = 0.01f;     // Length, [m]
    float time = 0.00f;     // Elapsed Time, [s]

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

    // Window
    Window window(1600, 900, "OpenGL");

    // Import shaders
    Shader shader("res/default.vert", "res/default.frag");

    // Setup initial condition
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            rho0[INDEX(x, y)] = 100.0f * sin(0.1f*x) * sin(0.1f*y);
        }
    }

    // Run main loop
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(window.window)) {

        // Get input events
        glfwPollEvents();

        // Setup simulation
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                Vx0[INDEX(x, y)] = velocity;
                Vy0[INDEX(x, y)] = velocity;

                float S = -sin(0.1f*x) * sin(0.1f*y) * sin(time) \
                        + 0.1f * velocity * cos(0.1f*x) * sin(0.1f*y) * cos(time) \
                        + 0.1f * velocity * sin(0.1f*x) * cos(0.1f*y) * cos(time) \
                        + 0.01f * diff * sin(0.1f*x) * sin(0.1f*y) * cos(time) \
                        + 0.01f * diff * sin(0.1f*x) * sin(0.1f*y) * cos(time);

                rho[INDEX(x, y)] = rho0[INDEX(x, y)] + 100.0f * dt * S;
            }
        }

        // Swap buffers
        SWAP(rho, rho0);

        // Apply B.C.
        for (int x = 0; x < N; x++) {
            rho[INDEX(x,   0)] = 100.0f * sin(0.1f*x) * sin(0.1f*0) * cos(time);
            rho[INDEX(x, N-1)] = 100.0f * sin(0.1f*x) * sin(0.1f*N) * cos(time);
        }

        for (int y = 0; y < N; y++) {
            rho[INDEX(  0, y)] = 100.0f * sin(0.1f*0) * sin(0.1f*y) * cos(time);
            rho[INDEX(N-1, y)] = 100.0f * sin(0.1f*N) * sin(0.1f*y) * cos(time);
        }

        // Run simulation
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
                }
            }
        }

        // Swap buffers
        SWAP(rho, rho0);

        // Calculate Error
        float errorRMS = 0.0f;

        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                
                // Manufactured and Exact solutions
                float rhoMan = 100.0f * sin(0.1f*x) *  sin(0.1f*y) * cos(time);
                float rhoDisc = rho0[INDEX(x, y)];
                
                // RMS Error
                errorRMS += pow(rhoMan - rhoDisc, 2);
            }
        }

        printf("RMS Error: %f\n", sqrt(errorRMS/pow(N, 2)));

        // Update time
        time += dt;

        // Draw density field
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                float x0 = (2.0f/N) * x - 1.0f;
                float y0 = (2.0f/N) * y - 1.0f; 
                float x1 = (2.0f/N) * (x+1) - 1.0f;
                float y1 = (2.0f/N) * (y+1) - 1.0f;

                vertices[INDEX(x, y)*20 + 0*5 + 0] = x0;
                vertices[INDEX(x, y)*20 + 0*5 + 1] = y0;
                vertices[INDEX(x, y)*20 + 0*5 + 2] = rho0[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 0*5 + 3] = rho0[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 0*5 + 4] = rho0[INDEX(x, y)] / 100.0f;

                vertices[INDEX(x, y)*20 + 1*5 + 0] = x1;
                vertices[INDEX(x, y)*20 + 1*5 + 1] = y0;
                vertices[INDEX(x, y)*20 + 1*5 + 2] = rho0[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 1*5 + 3] = rho0[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 1*5 + 4] = rho0[INDEX(x, y)] / 100.0f;

                vertices[INDEX(x, y)*20 + 2*5 + 0] = x1;
                vertices[INDEX(x, y)*20 + 2*5 + 1] = y1;
                vertices[INDEX(x, y)*20 + 2*5 + 2] = rho0[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 2*5 + 3] = rho0[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 2*5 + 4] = rho0[INDEX(x, y)] / 100.0f;

                vertices[INDEX(x, y)*20 + 3*5 + 0] = x0;
                vertices[INDEX(x, y)*20 + 3*5 + 1] = y1;
                vertices[INDEX(x, y)*20 + 3*5 + 2] = rho0[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 3*5 + 3] = rho0[INDEX(x, y)] / 100.0f;
                vertices[INDEX(x, y)*20 + 3*5 + 4] = rho0[INDEX(x, y)] / 100.0f;

                indices[INDEX(x, y)*6 + 0] = INDEX(x, y)*4 + 0;
                indices[INDEX(x, y)*6 + 1] = INDEX(x, y)*4 + 1;
                indices[INDEX(x, y)*6 + 2] = INDEX(x, y)*4 + 2;
                indices[INDEX(x, y)*6 + 3] = INDEX(x, y)*4 + 2;
                indices[INDEX(x, y)*6 + 4] = INDEX(x, y)*4 + 3;
                indices[INDEX(x, y)*6 + 5] = INDEX(x, y)*4 + 0;
            }
        }

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

        // // Print FPS
        // printf("FPS: %.2f\n\n", 1.0f / (glfwGetTime() - lastTime));
        // lastTime = glfwGetTime();
    }

    // Free Memory
    delete Vx;
    delete Vy;
    delete rho;
    delete Vx0;
    delete Vy0;
    delete rho0;
    delete vertices;
    delete indices;

    shader.Delete();
    window.Delete();

    return EXIT_SUCCESS;
}
