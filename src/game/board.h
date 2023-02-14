#ifndef REVERSI_BOARD_H
#define REVERSI_BOARD_H

#include "../state.h"
#include "uint128_t.h"

namespace Game::Board {
    std::vector<Point> GetSurroundingCoordinates(Point p);
    Move GetValidDirectionsForCell(Point cell, char board[8][8], char color);
    uint128_t State(char board[8][8]);
    unsigned long Hash(char board[8][8]);
}

#endif //REVERSI_BOARD_H
