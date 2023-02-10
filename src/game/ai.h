#ifndef REVERSI_AI_H
#define REVERSI_AI_H

#include <random>
#include "reversi.h"
#include "board.h"

class SimulationContext {
public:
    SimulationContext();
    std::vector<Move> GetPositionInfo();
    char simBoard[8][8]{};
};

namespace Game::AI {
    Move GetBestMove();
    void PlayBestMove();

}

#endif //REVERSI_AI_H
