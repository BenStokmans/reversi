#include "state.h"

bool showDebugWindow = true;

int windowStartX = 100, windowStartY = 100;

int width = 800;
int height = 800;

bool playingLocal = true;
bool clientTurn = true;
bool clientIsWhite = true;
int boardSize = 8;
float gridLineWidth = 0.005f;
bool highlighted[8][8] = {};
char board[8][8] = {};

GLFWwindow* glfwWindow = nullptr;