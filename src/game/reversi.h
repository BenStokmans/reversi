#ifndef REVERSI_REVERSI_H
#define REVERSI_REVERSI_H

#include "../state.h"
#include "../ui/helper.h"
#include "board.h"

namespace Game {
    void Init();
    void PlayMove(const Move& move, char color);
    Move GetMove(Point point);
    std::vector<Move> GetPossibleMoves(char color);
}

#endif //REVERSI_REVERSI_H
