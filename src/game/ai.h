#ifndef REVERSI_AI_H
#define REVERSI_AI_H

#include <random>
#include "reversi.h"
#include "board.h"
#include "fastboard/fastboard.h"

class SimulationContext {
public:
    SimulationContext(char startColor, char board[8][8]);
    char playerColor{};
    char colorToPlay{};
    char simBoard[8][8]{};
};

namespace Game::AI {
    Move GetBestMove();
    void PlayBestMove();
}

#endif //REVERSI_AI_H
