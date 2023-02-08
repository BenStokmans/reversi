#include "state.h"

int windowStartX = 100, windowStartY = 100;

bool gameWindowOpen = false;
int width = 800;
int height = 800;

int boardSize = 8;
float gridLineWidth = 0.005f;
char board[8][8] = {};

GLFWwindow* glfwWindow = nullptr;