#ifndef REVERSI_REVERSI_H
#define REVERSI_REVERSI_H

#include "../state.h"
#include "../ui/helper.h"

namespace Game {
    void Init();
    void PlayMove(const Move& move);
    Move GetMove(Point point);
    std::vector<Move> GetPossibleMoves(char color);
}

#endif //REVERSI_REVERSI_H
