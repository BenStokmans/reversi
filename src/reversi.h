#ifndef REVERSI_REVERSI_H
#define REVERSI_REVERSI_H

#include "state.h"
#include "helper.h"

void initReversi();
void highLightPossibleMoves(Shader* shader);
void playMove(Point move);
bool moveIsValid(Point move);
std::vector<Point> getPossibleMoves();

#endif //REVERSI_REVERSI_H
