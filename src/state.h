#ifndef REVERSI_STATE_H
#define REVERSI_STATE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>

#define DEBUG

struct Point {
    int x,y;
};

struct Move {
    Point square;
    std::vector<Point> directions;
};

extern bool showDebugWindow;
extern bool showWinWindow;
extern bool winWindowFocus;
extern bool gameOver;

extern int windowStartX, windowStartY;

extern int width;
extern int height;

#define CURRENT_PLAYER (char)(clientTurn ? (clientIsWhite ? 2 : 1) : (clientIsWhite ? 1 : 2))
#define NEXT_PLAYER (char)(clientTurn ? (clientIsWhite ? 1 : 2) : (clientIsWhite ? 2 : 1))

#define LOCAL_PLAYER clientIsWhite ? 2 : 1

extern bool playingLocal;
extern bool clientTurn;
extern bool clientIsWhite;
extern int boardSize;
extern float gridLineWidth;
extern std::vector<Move> currentLegalMoves;
extern bool highlighted[8][8];
extern char board[8][8];

extern GLFWwindow* glfwWindow;

#endif //REVERSI_STATE_H
