#ifndef REVERSI_GAME_H
#define REVERSI_GAME_H

#include "ui.h"

void drawGameOver();
void showBestMove(Shader* cellShader, GLuint cellVAO, Shader* diskShader, GLuint diskVAO);
void drawDisks(Shader* shader);
void highlightPossibleMoves(Shader* shader);
void highLightModified(Shader* shader);

#endif //REVERSI_GAME_H
