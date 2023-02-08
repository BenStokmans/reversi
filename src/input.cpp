#include "input.h"

void bufferSizeCallback([[maybe_unused]] GLFWwindow* win, int w, int h) {
    glViewport(0, 0, w, h);
}

void keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, int mods) {
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q || (key == GLFW_KEY_F4 && GLFW_MOD_ALT & mods)) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouseCallback(GLFWwindow* window, int button, [[maybe_unused]] int action, [[maybe_unused]] int mods) {
    // if we are mousing over an ImGui window don't process this input
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.WantCaptureMouse) return;

    // get cursor position
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    // 0, 0 is top left of window
    // 800, 800 is bottom right of window
    // our grid has the y-axis inverted compared to the mouse coordinates, so we'll account for that
    y = height - y;
    int cellX = floor(x / (width / (double)boardSize));
    int cellY = floor(y / (height / (double)boardSize));

    board[cellX][cellY] = button == 0 ? 1 : 2;
    if (button == 3) board[cellX][cellY] = 0;
}