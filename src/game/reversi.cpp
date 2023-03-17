#include "reversi.h"

void Game::Init() {
    // reset game over popup
    gameStarted = false;
    winWindowFocus = true;

    // clear data from previous game
    modifiedCells = 0;
    currentLegalMoves.clear();
    clientIsWhite = false;
    clientTurn = true;
    if (gameMode == GameMode::AI) {
        clientIsWhite = aiColor == 1;
        if (clientIsWhite)
            clientTurn = false;
    }

    // reset board
    gameBoard.Reset();
}