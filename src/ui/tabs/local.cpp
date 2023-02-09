#include "local.h"

void LocalTab::Draw() {
    if (!ImGui::BeginTabItem("Local"))
        return;

    ImGui::Dummy(ImVec2(4,4));
    if (ImGui::Button("Restart game")) {
        Game::Init();
        if (clientIsWhite) {
            Game::AI::PlayBestMove(aiDifficulty, aiColor, aiDepth);
            gameStarted = true;
        }
    }
    ImGui::Separator();
    if (aiManual && aiEnabled) {
        ImGui::BeginDisabled(CURRENT_PLAYER != aiColor || !gameStarted);
        if (ImGui::Button("Play next AI move"))
            Game::AI::PlayBestMove(aiDifficulty, aiColor, aiDepth);
        ImGui::EndDisabled();
    }
    ImGui::Checkbox("Play against computer", &aiEnabled);

    ImGui::BeginDisabled(!aiEnabled);
    ImGui::Checkbox("Step AI manually", &aiManual);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!aiEnabled || gameStarted);
    const char* colorItems[] = { "Black", "White" };
    if (ImGui::BeginCombo("AI Color", aiColorStr)) {
        for (int n = 0; n < IM_ARRAYSIZE(colorItems); n++)
        {
            bool selected = (aiColorStr == colorItems[n]);
            if (ImGui::Selectable(colorItems[n], selected)) {
                aiColorStr = colorItems[n];
                aiColor = n + 1; // NOLINT(cppcoreguidelines-narrowing-conversions)
                clientIsWhite = aiColor == 1;
                if (clientIsWhite && !gameStarted) {
                    clientTurn = false;
                    Game::AI::PlayBestMove(aiDifficulty, aiColor, aiDepth);
                    gameStarted = true;
                }
                currentLegalMoves.clear();
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    const char* aiItems[] = { "Random", "Easy", "Normal", "Difficult" };
    if (ImGui::BeginCombo("AI difficulty", aiDifficultyStr)) {
        for (int n = 0; n < IM_ARRAYSIZE(aiItems); n++)
        {
            bool selected = (aiDifficultyStr == aiItems[n]);
            if (ImGui::Selectable(aiItems[n], selected)) {
                aiDifficultyStr = aiItems[n];
                aiDifficulty = static_cast<AiDifficulty>(n);
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!aiEnabled || aiDifficulty == AiDifficulty::Random);
    ImGui::SliderInt("Depth", &aiDepth, 1, 30, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::EndDisabled();
    ImGui::EndTabItem();
}