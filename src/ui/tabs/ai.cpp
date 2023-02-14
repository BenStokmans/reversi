#include "ai.h"

void AITab::Draw() {
    if (!ImGui::BeginTabItem("AI"))
        return;

    ImGui::Dummy(ImVec2(4,4));
    const char* aiItems[] = { "Random", "Easy", "Normal", "Hard" };
    if (ImGui::BeginCombo("AI difficulty", aiDifficultyStr)) {
        for (int n = 0; n < IM_ARRAYSIZE(aiItems); n++) {
            bool selected = (aiDifficultyStr == aiItems[n]);
            if (ImGui::Selectable(aiItems[n], selected)) {
                aiDifficultyStr = aiItems[n];
                aiDifficulty = static_cast<AiDifficulty>(n);
                cachedAiMove = {};
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::BeginDisabled(aiDifficulty != AiDifficulty::Hard);
    ImGui::SliderInt("Depth", &aiDepth, 1, 15, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::EndDisabled();

    ImGui::Separator();

    if (gameMode == GameMode::Local) {
        ImGui::Checkbox("Show best move according to AI", &showAiMove);
        ImGui::EndTabItem();
        return;
    }

    if (aiManual) {
        ImGui::BeginDisabled(CURRENT_PLAYER != aiColor || !gameStarted);
        if (ImGui::Button("Play next AI move"))
            Game::AI::PlayBestMove();
        ImGui::EndDisabled();
    }
    ImGui::Checkbox("Step AI manually", &aiManual);

    const char* colorItems[] = { "Black", "White" };
    if (ImGui::BeginCombo("AI Color", aiColorStr)) {
        for (int n = 0; n < IM_ARRAYSIZE(colorItems); n++) {
            bool selected = (aiColorStr == colorItems[n]);
            if (ImGui::Selectable(colorItems[n], selected)) {
                aiColorStr = colorItems[n];
                if (aiColor != n+1) {
                    aiColor = n + 1; // NOLINT(cppcoreguidelines-narrowing-conversions)
                    Game::Init();
                }
                clientIsWhite = aiColor == 1;
                if (clientIsWhite && !gameStarted) {
                    Game::AI::PlayBestMove();
                }
                currentLegalMoves.clear();
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::EndTabItem();
}