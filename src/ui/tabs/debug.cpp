#include "debug.h"

void DebugTab::Draw() {
    if (!ImGui::BeginTabItem("Debug"))
        return;
    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Q)) {
        glfwSetWindowShouldClose(glfwWindow, true);
    }

    // TODO: just keep track of these counts or compute them after every move so we dont have to do this every frame
    int whiteDisks = 0, blackDisks = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            char side = board[i][j];
            if (side == 1) blackDisks++;
            if (side == 2) whiteDisks++;
        }
    }
    ImGui::Text("Turn: %s", CURRENT_PLAYER == 2 ? "white" : "black");
    ImGui::Text("White disks: %d", whiteDisks);
    ImGui::Text("Black disks: %d", blackDisks);
    ImGui::Text("Game over: %s", gameOver ? "true" : "false");

    ImGui::BeginDisabled(aiEnabled);

    if (ImGui::Checkbox("Client is white", &clientIsWhite)) {
        aiColor = clientIsWhite ? 1 : 2;
        aiColorStr = clientIsWhite ? "Black" : "White";
        currentLegalMoves.clear();
    }
    if (ImGui::Checkbox("Client to play", &clientTurn)) currentLegalMoves.clear();

    ImGui::EndDisabled();
    ImGui::EndTabItem();
}
