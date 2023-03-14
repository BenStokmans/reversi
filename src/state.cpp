#include "state.h"
#include "game/fastboard/fastboard.h"

bool showAiMove = false;
Move cachedAiMove = {};

int aiDiskMul = 4; // weight = 1.0
int aiEdgeDiskMul = 5; // weight = 1.25
int aiAdjacentCornerDiskMul = 3; // weight = 0.75
int aiCornerDiskMul = 6; // weight = 1.5

bool aiManual = false;
const char* aiDifficultyStr = "Random";
AiDifficulty aiDifficulty = AiDifficulty::Random;
const char* aiColorStr = "White";
char aiColor = 2; // white
int aiDepth = 10;

bool showEval = true;
int quiescenceSearchLim = 25;
bool quiescenceSearchEnabled = true;
std::string currentEvalText = "Eval: uninitialized";
std::unordered_map<uint64_t, Move> bestMoveNow = std::unordered_map<uint64_t, Move>();
std::unordered_map<uint64_t, Move> bestMoveNext = std::unordered_map<uint64_t, Move>();
bool enableEvalBar = true;
float evalBarValueGoal = 0.f;
float evalBarAnimationRate = 0.01f;
float evalBarValue = 0.f;

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

uint64_t modifiedCells;
std::vector<Move> currentLegalMoves;
bool highlighted[8][8] = {};
FastBoard gameBoard;

GLFWwindow* glfwWindow = nullptr;