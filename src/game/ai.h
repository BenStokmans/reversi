#ifndef REVERSI_AI_H
#define REVERSI_AI_H

#include <random>
#include "reversi.h"

namespace Game::AI {
    Move GetBestMove(AiDifficulty difficulty, char color, int depth = 0);
    void PlayBestMove(AiDifficulty difficulty, char color, int depth = 0);

}

#endif //REVERSI_AI_H
