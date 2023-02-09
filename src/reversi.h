#ifndef REVERSI_REVERSI_H
#define REVERSI_REVERSI_H

#include "state.h"
#include "helper.h"

void initReversi();
void highLightPossibleMoves(Shader* shader);
void playMove(const Move& move);
Move getMove(Point point);
std::vector<Move> getPossibleMoves(char color);

#endif //REVERSI_REVERSI_H
