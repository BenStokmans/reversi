#ifndef REVERSI_AI_H
#define REVERSI_AI_H

#include <random>
#include "reversi.h"
#include "fastboard/fastboard.h"

namespace Game::AI {
    Move GetBestMove();
    void PlayBestMove();

    int_fast8_t GetEval(int depth);
    int_fast8_t GetEndGame(int depth);
}
int_fast8_t minmax(FastBoard board, int_fast8_t alpha, int_fast8_t beta, int depth, bool max);
Move getMoveMinMax(FastBoard gameState, int depth);

#endif //REVERSI_AI_H
