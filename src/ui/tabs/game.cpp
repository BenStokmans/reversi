#include "game.h"

void LocalTab::Draw() {
    if (!ImGui::BeginTabItem("Game"))
        return;

    ImGui::Dummy(ImVec2(4,4));
    if (ImGui::Button("Restart game")) {
        Game::Init();
        if (clientIsWhite && gameMode == GameMode::AI && !aiManual)
            Game::AI::PlayBestMove();
    }
    ImGui::Separator();
    const char* gameModeItems[] = { "Local", "vs AI", "Online" };
    if (ImGui::BeginCombo("Game mode", gameModeStr)) {
        for (int n = 0; n < IM_ARRAYSIZE(gameModeItems); n++) {
            bool selected = (gameModeStr == gameModeItems[n]);
            if (ImGui::Selectable(gameModeItems[n], selected)) {
                gameModeStr = gameModeItems[n];
                gameMode = static_cast<GameMode>(n);

                Game::Init();
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (gameMode == GameMode::Online) {
        quiescenceSearchEnabled = false;
        showEval = false;
        enableEvalBar = false;
    }
    ImGui::Separator();
    ImGui::BeginDisabled(gameMode == GameMode::Online);
    ImGui::Checkbox("Show Eval", &showEval);
    if (showEval) {
        ImGui::Text("%s", currentEvalText.c_str());
    }
    if (ImGui::Checkbox("Show eval bar", &enableEvalBar)) {
        if (enableEvalBar) {
            Game::AI::StartEvalBarAnimation();
        }
    }
    ImGui::EndDisabled();
    ImGui::EndTabItem();
}