#ifndef REVERSI_BOARD_H
#define REVERSI_BOARD_H

#include "../state.h"
#ifdef WIN32
#include "uint128_t.h"
#endif

namespace Game::Board {
    std::vector<Point> GetSurroundingCoordinates(Point p);
    Move GetValidDirectionsForCell(Point cell, char board[8][8], char color);
#ifdef WIN32
    uint128_t State(char board[8][8]);
#else
    unsigned __int128 State(char board[8][8]);
#endif
    unsigned long Hash(char board[8][8]);
}

#endif //REVERSI_BOARD_H
