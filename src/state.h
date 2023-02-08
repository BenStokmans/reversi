#ifndef REVERSI_STATE_H
#define REVERSI_STATE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

extern bool showDebugWindow;

extern int windowStartX, windowStartY;

extern int width;
extern int height;

extern bool playingLocal;
extern bool clientTurn;
extern bool clientIsWhite;
extern int boardSize;
extern float gridLineWidth;
extern bool highlighted[8][8];
extern char board[8][8];

extern GLFWwindow* glfwWindow;

#endif //REVERSI_STATE_H
