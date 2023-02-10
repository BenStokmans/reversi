#include "game.h"

void drawDisks(Shader* shader) {
    float diskSize = 1.0f / (float)boardSize;

    double mouseX, mouseY;
    glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
    auto hoverCell = screenToCellCoords(mouseX, mouseY);

    // draw all disks on the board and the hovered cell if it's our turn
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            char side = gameBoard[i][j];

            bool renderHover = i == hoverCell.x && j == hoverCell.y && clientTurn;
            if (side == 0 && !renderHover) continue;

            float x = (-1 + diskSize) + (float)i * diskSize * 2;
            float y = (-1 + diskSize) + (float)j * diskSize * 2;

            auto color = glm::vec4(0, 0, 0, 1.f);
            if (side == 2) {
                color = glm::vec4(1);
            }

            if (renderHover && side == 0) {
                color = glm::vec4(glm::vec3(clientIsWhite ? 1 : 0), 0.3);
            }
            shader->set("centre", glm::vec2(x,y));
            shader->set("color", color);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
    }
}

void highlightPossibleMoves(Shader* shader) {
    if (!highlightPossibleCells || !clientTurn) return;
    float cellSize = 1.0f / (float)boardSize;

    shader->set("color", highlightPossibleColor);
    auto possibleMoves = Game::GetPossibleMoves(LOCAL_PLAYER);
    for (const auto& move : possibleMoves) {
        float x = (-1 + cellSize) + (float)move.cell.x * cellSize * 2;
        float y = (-1 + cellSize) + (float)move.cell.y * cellSize * 2;
        shader->set("centre", glm::vec2(x,y));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}

void highLightModified(Shader* shader) {
    if (!highlightModifiedCells) return;
    float cellSize = 1.0f / (float)boardSize;

    shader->set("color", highlightModifiedColor);
    for (const auto& cell : modifiedCells) {
        float x = (-1 + cellSize) + (float)cell.x * cellSize * 2;
        float y = (-1 + cellSize) + (float)cell.y * cellSize * 2;
        shader->set("centre", glm::vec2(x,y));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}

void drawGameOver() {
    if (winWindowFocus) {
        winWindowFocus = false;
        ImGui::SetNextWindowFocus();
    }
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing);

    int whiteDisks = 0, blackDisks = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            char side = gameBoard[i][j];
            if (side == 1) blackDisks++;
            if (side == 2) whiteDisks++;
        }
    }
    ImGui::Begin("Game over", &showWinWindow);
    ImGui::Text("%s has won the game!", whiteDisks > blackDisks ? "white" : "black");
    ImGui::End();
}

void showBestMove(Shader* cellShader, GLuint cellVAO, Shader* diskShader, GLuint diskVAO) {
    aiColor = CURRENT_PLAYER;
    Move move = cachedAiMove;
    if (move.boardState != Game::Board::State(gameBoard)) {
        move = Game::AI::GetBestMove();
        cachedAiMove = move;
    }

    float cellSize = 1.0f / (float)boardSize;
    float x = (-1 + cellSize) + (float)move.cell.x * cellSize * 2;
    float y = (-1 + cellSize) + (float)move.cell.y * cellSize * 2;

    if (highlightAiMove) {
        cellShader->use();
        glBindVertexArray(cellVAO);
        cellShader->set("centre", glm::vec2(x, y));
        cellShader->set("color", highlightAiColor);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    auto diskColor = glm::vec4(0, 0, 0, 0.5f);
    if (CURRENT_PLAYER == 2) {
        diskColor = glm::vec4(1, 1, 1, 0.5f);
    }

    diskShader->use();
    glBindVertexArray(diskVAO);
    diskShader->set("centre", glm::vec2(x, y));
    diskShader->set("color", diskColor);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
