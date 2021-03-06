// #include "Simulation.h"

// #define N 256
// #define ITER 16

// #define INDEX(X, Y) ((Y) * N + (X))
// #define SWAP(A, B) {float* C = A; A = B; B = C;}


//     // Physical Quantities
//     // float diff = 1E-7;      // Diffusivity, [m^2/s]
//     // float velocity = 0.001; // Velocity, [m/s]
//     float size = 0.01f;     // Length, [m]
//     float time = 0.00f;     // Elapsed Time, [s]

//     float velocity = 0.0f;
//     float diff = 0.0f;

//     // Discretizations
//     float dx = size / N;
//     float dy = size / N;
//     float dt = 0.01f;

//     // Allocate memory
//     float* Vx = new float[N*N]();
//     float* Vy = new float[N*N]();
//     float* rho = new float[N*N]();

//     float* Vx0 = new float[N*N]();
//     float* Vy0 = new float[N*N]();
//     float* rho0 = new float[N*N]();

//     // Draw Buffers
//     float* vertices = new float[4*5 * N*N]();
//     GLuint* indices = new GLuint[6 * N*N]();

    
//     // Setup initial condition
//     for (int y = 0; y < N; y++) {
//         for (int x = 0; x < N; x++) {
//             rho0[INDEX(x, y)] = 100.0f * sin(0.1f*x) * sin(0.1f*y);
//         }
//     }



//         // Setup simulation
//         for (int y = 0; y < N; y++) {
//             for (int x = 0; x < N; x++) {
//                 Vx0[INDEX(x, y)] = velocity;
//                 Vy0[INDEX(x, y)] = velocity;

//                 float S = -sin(0.1f*x) * sin(0.1f*y) * sin(time) \
//                         + 0.1f * velocity * cos(0.1f*x) * sin(0.1f*y) * cos(time) \
//                         + 0.1f * velocity * sin(0.1f*x) * cos(0.1f*y) * cos(time) \
//                         + 0.01f * diff * sin(0.1f*x) * sin(0.1f*y) * cos(time) \
//                         + 0.01f * diff * sin(0.1f*x) * sin(0.1f*y) * cos(time);

//                 rho[INDEX(x, y)] = rho0[INDEX(x, y)] + 100.0f * dt * S;
//             }
//         }

//         // Swap buffers
//         SWAP(rho, rho0);

//         // Run simulation
//         for (int i = 0; i < ITER; i++) {
//             for (int y = 1; y < N-1; y++) {
//                 for (int x = 1; x < N-1; x++) {
                    
//                     // Convective terms
//                     float convX = -Vx0[INDEX(x, y)] * (rho[INDEX(x+1, y)] - rho[INDEX(x-1, y)]) / (2*dx);
//                     float convY = -Vy0[INDEX(x, y)] * (rho[INDEX(x, y+1)] - rho[INDEX(x, y-1)]) / (2*dy);

//                     // Diffusive terms
//                     float diffX = (diff / (dx*dx)) * (rho[INDEX(x+1, y)] + rho[INDEX(x-1, y)]);
//                     float diffY = (diff / (dy*dy)) * (rho[INDEX(x, y+1)] + rho[INDEX(x, y-1)]);

//                     // Scaling factor
//                     float c = 1 + (2*diff*dt/(dx*dx)) + (2*diff*dt/(dy*dy));

//                     // Update density
//                     rho[INDEX(x, y)] = (1/c) * (rho0[INDEX(x, y)] + dt * (convX + convY + diffX + diffY));
//                 }
//             }
//         }

//         // Swap buffers
//         SWAP(rho, rho0);

//         // Calculate Error
//         float errorRMS = 0.0f;

//         for (int y = 0; y < N; y++) {
//             for (int x = 0; x < N; x++) {
                
//                 // Manufactured and Exact solutions
//                 float rhoMan = 100.0f * sin(0.1f*x) *  sin(0.1f*y) * cos(time);
//                 float rhoDisc = rho0[INDEX(x, y)];
                
//                 // RMS Error
//                 errorRMS += pow(rhoMan - rhoDisc, 2);
//             }
//         }

//         printf("RMS Error: %f\n", sqrt(errorRMS/pow(N, 2)));

//         // Update time
//         time += dt;

//         // Draw density field
//         for (int y = 0; y < N; y++) {
//             for (int x = 0; x < N; x++) {
//                 float x0 = (2.0f/N) * x - 1.0f;
//                 float y0 = (2.0f/N) * y - 1.0f; 
//                 float x1 = (2.0f/N) * (x+1) - 1.0f;
//                 float y1 = (2.0f/N) * (y+1) - 1.0f;

//                 float S = 1.0f;
//                 float L = 0.5f;

//                 vertices[INDEX(x, y)*20 + 0*5 + 0] = x0;
//                 vertices[INDEX(x, y)*20 + 0*5 + 1] = y0;
//                 vertices[INDEX(x, y)*20 + 0*5 + 2] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).r;
//                 vertices[INDEX(x, y)*20 + 0*5 + 3] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).g;
//                 vertices[INDEX(x, y)*20 + 0*5 + 4] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).b;

//                 vertices[INDEX(x, y)*20 + 1*5 + 0] = x1;
//                 vertices[INDEX(x, y)*20 + 1*5 + 1] = y0;
//                 vertices[INDEX(x, y)*20 + 1*5 + 2] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).r;
//                 vertices[INDEX(x, y)*20 + 1*5 + 3] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).g;
//                 vertices[INDEX(x, y)*20 + 1*5 + 4] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).b;

//                 vertices[INDEX(x, y)*20 + 2*5 + 0] = x1;
//                 vertices[INDEX(x, y)*20 + 2*5 + 1] = y1;
//                 vertices[INDEX(x, y)*20 + 2*5 + 2] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).r;
//                 vertices[INDEX(x, y)*20 + 2*5 + 3] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).g;
//                 vertices[INDEX(x, y)*20 + 2*5 + 4] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).b;

//                 vertices[INDEX(x, y)*20 + 3*5 + 0] = x0;
//                 vertices[INDEX(x, y)*20 + 3*5 + 1] = y1;
//                 vertices[INDEX(x, y)*20 + 3*5 + 2] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).r;
//                 vertices[INDEX(x, y)*20 + 3*5 + 3] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).g;
//                 vertices[INDEX(x, y)*20 + 3*5 + 4] = HSL((100.0f + rho0[INDEX(x, y)]) / 1200.0f, S, L).b;

//                 indices[INDEX(x, y)*6 + 0] = INDEX(x, y)*4 + 0;
//                 indices[INDEX(x, y)*6 + 1] = INDEX(x, y)*4 + 1;
//                 indices[INDEX(x, y)*6 + 2] = INDEX(x, y)*4 + 2;
//                 indices[INDEX(x, y)*6 + 3] = INDEX(x, y)*4 + 2;
//                 indices[INDEX(x, y)*6 + 4] = INDEX(x, y)*4 + 3;
//                 indices[INDEX(x, y)*6 + 5] = INDEX(x, y)*4 + 0;
//             }
//         }
// 
// // Free Memory
// delete Vx;
// delete Vy;
// delete rho;
// delete Vx0;
// delete Vy0;
// delete rho0;
// delete vertices;
// delete indices;