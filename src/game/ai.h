#ifndef REVERSI_AI_H
#define REVERSI_AI_H

#include <random>
#include "reversi.h"
#include "fastboard/fastboard.h"

namespace Game::AI {
    Move GetBestMove();
    void PlayBestMove();

    int GetEval(int depth);
    int GetEndGame(int depth);
}
int minmax(FastBoard board, int alpha, int beta, int depth, bool max);
Move getMoveMinMax(FastBoard gameState, int depth);

#endif //REVERSI_AI_H
