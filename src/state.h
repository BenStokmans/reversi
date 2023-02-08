#ifndef REVERSI_STATE_H
#define REVERSI_STATE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

extern int windowStartX, windowStartY;

extern int width;
extern int height;

extern int boardSize;
extern float gridLineWidth;
extern bool highlighted[8][8];
extern char board[8][8];

extern GLFWwindow* glfwWindow;

#endif //REVERSI_STATE_H
