#include "online.h"

void OnlineTab::Draw() {
    if (!ImGui::BeginTabItem("Online"))
        return;
    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Q)) {
        glfwSetWindowShouldClose(glfwWindow, true);
    }
    char* ipBuffer[20]{ const_cast<char*>(ip.c_str()) };
    if (ImGui::InputText("ip", *ipBuffer, IM_ARRAYSIZE(ipBuffer))) {
        ip = *ipBuffer;
    }
    ImGui::SameLine();
    if (ImGui::Button("Paste")) {
        ip = ImGui::GetClipboardText();
        Client::Connect();
    }
    ImGui::BeginDisabled(connected);
    if (ImGui::Button("Connect")) {
        Client::Connect();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(!connected);
    if (ImGui::Button("Disconnect")) {
        Client::Disconnect();
    }
    ImGui::EndDisabled();
    ImGui::Separator();

    ImGui::BeginDisabled(onlineGameId != 0);
    char* nameBuffer[20]{ const_cast<char*>(username.c_str()) };
    if (ImGui::InputText("Username", *nameBuffer, IM_ARRAYSIZE(nameBuffer))) {
        username = *nameBuffer;
    }

    const char* colorItems[] = { "Random", "Black", "White" };
    if (ImGui::BeginCombo("Color", onlineColorStr)) {
        for (int n = 0; n < IM_ARRAYSIZE(colorItems); n++) {
            bool selected = (onlineColorStr == colorItems[n]);
            if (ImGui::Selectable(colorItems[n], selected)) {
                onlineColorStr = colorItems[n];
                if (onlineColor != n) {
                    onlineColor = reversi::Color(n); // NOLINT(cppcoreguidelines-narrowing-conversions)
                }
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Separator();
    if (ImGui::Button("Create game"))
        Client::CreateGame();
    ImGui::SameLine();
    if (ImGui::Button("Join game")) {
        Client::JoinGame();
    }
    ImGui::SameLine();
    ImGui::BeginDisabled(onlineGameId == 0);
    if (ImGui::Button("Leave game"))
        Client::LeaveGame();
    ImGui::EndDisabled();
    ImGui::Separator();
    char* gameIdBuffer[20]{ const_cast<char*>(gameId.c_str()) };
    if (ImGui::InputText("Game ID", *gameIdBuffer, IM_ARRAYSIZE(gameIdBuffer))) {
        gameId = *gameIdBuffer;
    }
    ImGui::SameLine();
    if (ImGui::Button("Paste")) {
        gameId = ImGui::GetClipboardText();
        Client::JoinGame();
    }

    ImGui::EndDisabled();

    ImGui::Text("Game ID: %llu", onlineGameId);
    ImGui::SameLine();
    if (ImGui::Button("Copy"))
        ImGui::SetClipboardText(std::to_string(onlineGameId).c_str());

    ImGui::Text("Assigned color: %s", assignedColor == 0 ? "none" : assignedColor == reversi::Color::BLACK ? "black" : "white");
    ImGui::EndTabItem();
}

void drawOnlineError() {
    ImGui::SetNextWindowFocus();

    ImGui::SetNextWindowSize({300, 100}, ImGuiCond_Appearing);
    auto center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos({center.x - 150, center.y - 50}, ImGuiCond_Appearing);

    ImGui::Begin("Dialog", &showWinWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        onlineError = false;
        onlineErrorStr = "";
    }

    centeredText(onlineErrorStr.c_str());

    if (centeredButton("OK")) {
        onlineError = false;
        onlineErrorStr = "";
    }
    ImGui::End();
}
