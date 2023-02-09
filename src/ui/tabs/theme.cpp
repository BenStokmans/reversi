#include "theme.h"

void ThemeTab::Draw() {
    if (!ImGui::BeginTabItem("Theme"))
        return;

    ImGui::Dummy(ImVec2(4,4));
    // TODO: shorten name
    float highlightPossibleColorBuffer[4] = {
            (float)highlightPossibleColor.r / 255.f,
            (float)highlightPossibleColor.g / 255.f,
            (float)highlightPossibleColor.b / 255.f,
            (float)highlightPossibleColor.a / 255.f
    };
    ImGui::Checkbox("Highlight legal moves", &highlightPossibleCells);
    if (ImGui::ColorEdit4("1",highlightPossibleColorBuffer, ImGuiColorEditFlags_NoLabel)) {
        highlightPossibleColor = {
                (unsigned char)(highlightPossibleColorBuffer[0]*255.f),
                (unsigned char)(highlightPossibleColorBuffer[1]*255.f),
                (unsigned char)(highlightPossibleColorBuffer[2]*255.f),
                (unsigned char)(highlightPossibleColorBuffer[3]*255.f)
        };
    }
    ImGui::Separator();
    float highlightModifiedColorBuffer[4] = {
            (float)highlightModifiedColor.r / 255.f,
            (float)highlightModifiedColor.g / 255.f,
            (float)highlightModifiedColor.b / 255.f,
            (float)highlightModifiedColor.a / 255.f
    };
    ImGui::Checkbox("Highlight modified disks", &highlightModifiedCells);
    if (ImGui::ColorEdit4("2",highlightModifiedColorBuffer, ImGuiColorEditFlags_NoLabel)) {
        highlightModifiedColor = {
                (unsigned char)(highlightModifiedColorBuffer[0]*255.f),
                (unsigned char)(highlightModifiedColorBuffer[1]*255.f),
                (unsigned char)(highlightModifiedColorBuffer[2]*255.f),
                (unsigned char)(highlightModifiedColorBuffer[3]*255.f)
        };
    }
    ImGui::EndTabItem();
}
