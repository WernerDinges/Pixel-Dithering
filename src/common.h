#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#define DEBUG_FLAG 0

#define PI                             3.14159265358979323846
#define SIMULATION_RATE              500
#define SIM_STEP_MILLIS                (1000 / SIMULATION_RATE)

// -------------------------------------
// |               Hash                |
// -------------------------------------

uint32_t hash(int x, int z) {
    uint32_t h = 2166136261u; // FNV offset
    h ^= x;  
    h *= 16777619;  
    h ^= z;  
    h *= 16777619;  
    return h;
}

// -------------------------------------
// |               World               |
// -------------------------------------

typedef struct World {
    
} World;

typedef struct Camera {
    float x, y, z;
    float frontX, frontY, frontZ;
    float pitch, yaw;
    float fov;
    float lastX, lastY;
    int firstMouse;
    float sensitivity;
    float speed;
} Camera;

#endif // COMMON_H