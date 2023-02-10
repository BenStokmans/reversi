#ifndef REVERSI_BOARD_H
#define REVERSI_BOARD_H

#include "../state.h"

namespace Game::Board {
    std::vector<Point> GetSurroundingCoordinates(Point p);
    Move GetValidDirectionsForCell(Point cell, char board[8][8], char color);
    __int128 State(char board[8][8]);
    long Hash(char board[8][8]);
}

#endif //REVERSI_BOARD_H
