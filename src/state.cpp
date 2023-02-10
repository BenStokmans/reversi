#include "state.h"

bool showAiMove = false;
Move cachedAiMove = {};

bool aiManual = false;
const char* aiDifficultyStr = "Random";
AiDifficulty aiDifficulty = AiDifficulty::Random;
const char* aiColorStr = "White";
char aiColor = 2; // white
int aiDepth = 1;

bool showDebugWindow = true;
bool showGameWindow = true;
bool showWinWindow = true;
bool winWindowFocus = true;
bool gameOver = true;

int windowStartX = 100, windowStartY = 100;

int width = 800;
int height = 800;

GameMode gameMode = GameMode::Local;
const char* gameModeStr = "Local";
bool gameStarted = false;
bool clientTurn = true;
bool clientIsWhite = false;
int boardSize = 8;
float gridLineWidth = 0.005f;

Color4 highlightPossibleColor = {6, 100, 6, 255};
bool highlightPossibleCells = true;
Color4 highlightModifiedColor = {255, 113, 116, 160};
bool highlightModifiedCells = true;
Color4 highlightAiColor = {50, 80, 150, 255};
bool highlightAiMove = false;

std::vector<Point> modifiedCells;
std::vector<Move> currentLegalMoves;
bool highlighted[8][8] = {};
char gameBoard[8][8] = {};

GLFWwindow* glfwWindow = nullptr;