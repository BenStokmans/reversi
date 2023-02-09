#include "state.h"

bool showDebugWindow = true;
bool showWinWindow = true;
bool winWindowFocus = true;
bool gameOver = true;

int windowStartX = 100, windowStartY = 100;

int width = 800;
int height = 800;

bool playingLocal = true;
bool clientTurn = true;
bool clientIsWhite = false;
int boardSize = 8;
float gridLineWidth = 0.005f;
std::vector<Move> currentLegalMoves;
bool highlighted[8][8] = {};
char board[8][8] = {};

GLFWwindow* glfwWindow = nullptr;