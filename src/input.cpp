#include "input.h"

void bufferSizeCallback([[maybe_unused]] GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, int mods) {
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q || (key == GLFW_KEY_F4 && GLFW_MOD_ALT & mods)) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    // if we are mousing over an ImGui window don't process this input
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.WantCaptureMouse) return;

    // action 0 indicates mouse up which we don't care about
    if (action == 0) return;

    // if it's not our turn not process click
    if (!clientTurn) return;

    // get cursor position as cell coordinate
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    auto cell = screenToCellCoords(x, y);

    if (!moveIsValid(cell)) return;
    playMove(cell);
    clientTurn = false;

    if (!playingLocal) return;
    clientTurn = true;
    clientIsWhite = !clientIsWhite;
}