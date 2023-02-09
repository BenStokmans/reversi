#ifndef REVERSI_AI_H
#define REVERSI_AI_H

#include <random>
#include "reversi.h"


Move getBestMove(AiDifficulty difficulty, char color, int depth = 0);

#endif //REVERSI_AI_H
