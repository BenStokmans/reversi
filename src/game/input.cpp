#include "input.h"
#include "src/online/client.h"

void bufferSizeCallback([[maybe_unused]] GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, int mods) {
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q || (key == GLFW_KEY_F4 && GLFW_MOD_ALT & mods)) {
        quiescenceSearchEnabled = false;
        glfwSetWindowShouldClose(window, true);
    }
}

void mouseCallback(GLFWwindow* window, [[maybe_unused]] int button, int action, [[maybe_unused]] int mods) {
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
    auto cell = screenToCellCoordinates(x, y);

    // check if the move is valid
    int pos = cell.y * 8 + cell.x;
    if ((gameBoard.Moves() & 1ULL << pos) == 0) return;

    if (!gameStarted) gameStarted = true;
    clientTurn = false;

    // play the move
    modifiedCells = 0;
    prevBoard = gameBoard.Clone();
    modifiedCells = gameBoard.Play(pos);
    bestMoveNow = bestMoveNext;

    if (gameMode == GameMode::Online) {
        Client::PlayMove(cell);
        return;
    }

    if (gameMode == GameMode::AI) {
        if (aiManual) return;

        std::thread thr(Game::AI::PlayBestMove);
        thr.detach();
        return;
    }

    clientTurn = true;
    if (gameBoard.Moves() == 0) {
        gameBoard.SwitchTurn();
        return;
    }
    clientIsWhite = !clientIsWhite;
}