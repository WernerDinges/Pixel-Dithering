#include "common.h"
#include "shader/load.c"
#include "helper/frame.c"
#include "helper/camera.c"

int main(void) {

    // ----------------------------- OpenGL prelude -----------------------------

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ----------------------------- Window initialization -----------------------------

    int screenWidth = 800;
    int screenHeight = 600;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Pixel fog", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);    // Resizable
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    // Hide cursor
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    // ----------------------------- Fullscreen scene quad -----------------------------

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Vertex attribute: position (vec2)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Unbind VAO
    glBindVertexArray(0);

    GLuint shaderProgram = createShaderProgram("resources/shaders/vertex.glsl", "resources/shaders/test.fs");
    if (shaderProgram == 0) {
        glfwTerminate();
        return -1;
    }

    // ----------------------------- Camera initialization -----------------------------

    Camera camera = init_camera(screenWidth, screenHeight);

    // Initialize last known mouse position
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    camera.lastX = (float)mouseX;
    camera.lastY = (float)mouseY;

    // ----------------------------- Main loop -----------------------------

    lastTime = glfwGetTime();    // Initialize timer
    float sim_timer = 0;
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        double currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        sim_timer += deltaTime * 1000.0f;
        if (DEBUG_FLAG) FPS(deltaTime);    // Output FPS to console

        // Simulation
        while (sim_timer >= SIM_STEP_MILLIS) {
            // Ignore the first frame to avoid large jump
            if (!camera.firstMouse) update_camera(window, &camera, &mouseX, &mouseY, SIM_STEP_MILLIS); else camera.firstMouse = 0;

            sim_timer -= SIM_STEP_MILLIS;
        }

        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // Apply the raymarch shader
        glUseProgram(shaderProgram);

        // Update screen size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glUniform2f(glGetUniformLocation(shaderProgram, "resolution"), (float)width, (float)height);
        glUniform3f(glGetUniformLocation(shaderProgram, "cameraPos"), camera.x, camera.y, camera.z);
        glUniform1f(glGetUniformLocation(shaderProgram, "pitch"), camera.pitch * PI / 180.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "yaw"), camera.yaw * PI / 180.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "fov"), camera.fov);

        // Bind the VAO and draw the quad
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // ----------------------------- Cleanup -----------------------------

    // OpenGL cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
    
}
