#include "debug.h"

void DebugTab::Draw() {
    if (!ImGui::BeginTabItem("Debug"))
        return;
    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Q)) {
        glfwSetWindowShouldClose(glfwWindow, true);
    }

    int whiteDisks = gameBoard.PlayerDisks(), blackDisks = gameBoard.OpponentDisks();
    if (CURRENT_PLAYER == 1) {
        int tmp = whiteDisks;
        whiteDisks = blackDisks;
        blackDisks = tmp;
    }

    ImGui::Text("Turn: %s", CURRENT_PLAYER == 2 ? "white" : "black");
    ImGui::Text("White disks: %d", whiteDisks);
    ImGui::Text("Black disks: %d", blackDisks);
    ImGui::Text("Game over: %s", gameOver ? "true" : "false");

    ImGui::Separator();
    if (ImGui::Checkbox("Enable Quiescence Search", &quiescenceSearchEnabled)) {
        if (quiescenceSearchEnabled) {
            Game::AI::StartSearch();
        }
    }
    ImGui::SliderInt("Quiescence Search Max", &quiescenceSearchLim, 1, 64, "%d", ImGuiSliderFlags_AlwaysClamp);

    ImGui::EndTabItem();
}
