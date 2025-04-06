#include "../common.h"

// Fullscreen quad
static float quadVertices[] = {
    // positions (x, y)
    -1.0f, -1.0f, // lower left
     1.0f, -1.0f, // lower right
     1.0f,  1.0f, // upper right

    -1.0f, -1.0f, // lower left
     1.0f,  1.0f, // upper right
    -1.0f,  1.0f  // upper left
};

// ----------------------------------------------------------------------------

Camera init_camera(int screenWidth, int screenHeight) {
    return (Camera){
        .x = 0.0, .y = 1.6, .z = 0.0,
        .frontX = 0.0, .frontY = 0.0, .frontZ = -1.0,
        .yaw = -90.0, .pitch = 0.0,
        .fov = 60.0,
        .lastX = screenWidth / 2, .lastY = screenHeight / 2,
        .firstMouse = true,
        .sensitivity = 0.8,
        .speed = 4.7
    };
}

// Function to normalize a 3D vector
void normalize_vector(float v[3]) {
    float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length != 0.0f) {  // Prevent division by zero
        v[0] /= length;
        v[1] /= length;
        v[2] /= length;
    }
}

void update_camera(GLFWwindow* window, Camera* camera, double *mouseX, double *mouseY, float dt) {

    // ------------------------------ Mouse rotation ------------------------------

    // Poll mouse position
    glfwGetCursorPos(window, mouseX, mouseY);
    float offsetX = (float) *mouseX - camera->lastX;
    float offsetY = camera->lastY - (float) *mouseY;
    camera->lastX = (float) *mouseX;
    camera->lastY = (float) *mouseY;

    // Apply sensitivity scaling
    offsetX *= camera->sensitivity * dt * 0.1;
    offsetY *= camera->sensitivity * dt * 0.1;

    // Update yaw and pitch
    camera->yaw -= offsetX;
    camera->pitch += offsetY;

    // Clamp pitch to prevent flipping
    if (camera->pitch > 89.9f) camera->pitch = 89.9f;
    if (camera->pitch < -89.9f) camera->pitch = -89.9f;

    // Clamp yaw to prevent overflow
    if (camera->yaw > 360.0f) while (camera->yaw > 360.0f) camera->yaw -= 360.0f;
    if (camera->yaw < 0.0f) while (camera->yaw < 0.0f) camera->yaw += 360.0f;

    // Convert yaw/pitch to direction vector
    float radYaw = camera->yaw * (float)PI / 180.0f;
    float radPitch = camera->pitch * (float)PI / 180.0f;
    camera->frontX = cos(radYaw) * cos(radPitch);
    camera->frontY = sin(radPitch);
    camera->frontZ = sin(radYaw) * cos(radPitch);

    // Normalize the direction vector
    float front[3] = {camera->frontX, camera->frontY, camera->frontZ};
    normalize_vector(front);
    camera->frontX = front[0];
    camera->frontY = front[1];
    camera->frontZ = front[2];

    // ------------------------------ Keyboard ------------------------------

    // Esc -> Close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    // WASD movement
    int forward = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    int right = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    if (forward || right) {
        float alpha = 0.0;
        if (( forward ==  1) && (!right      )) alpha = 0.0;
        if (( forward ==  1) && ( right == -1)) alpha = 45.0;
        if ((!forward      ) && ( right == -1)) alpha = 90.0;
        if (( forward == -1) && ( right == -1)) alpha = 135.0;
        if (( forward == -1) && (!right      )) alpha = 180.0;
        if (( forward == -1) && ( right ==  1)) alpha = 225.0;
        if ((!forward      ) && ( right ==  1)) alpha = 270.0;
        if (( forward ==  1) && ( right ==  1)) alpha = 315.0;

        float rad = (camera->yaw + alpha) * PI / 180.0f;
        float dx = sinf(rad) * camera->speed * dt / 1000.0f;
        float dz = cosf(rad) * camera->speed * dt / 1000.0f;
        
        camera->x += dx;
        camera->z += dz;
    }

}