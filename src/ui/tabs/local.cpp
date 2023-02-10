#include "local.h"

void LocalTab::Draw() {
    if (!ImGui::BeginTabItem("Local"))
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
        for (int n = 0; n < IM_ARRAYSIZE(gameModeItems); n++)
        {
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
    ImGui::EndTabItem();
}