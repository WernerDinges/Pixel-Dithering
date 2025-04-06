#include "../common.h"

// FPS counter variables
float lastTime = 0.0;
double fpsTimer = 0.0;
int frameCount = 0;

void FPS(float dt) {
    frameCount++;
    fpsTimer += dt;

    if (fpsTimer >= 1.0) {    // One second has passed
        printf("|| FPS: %d\n", frameCount);
        frameCount = 0;
        fpsTimer = 0.0;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}