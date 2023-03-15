#ifndef REVERSI_STATE_H
#define REVERSI_STATE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "game/fastboard/fastboard.h"
#include <vector>
#include <unordered_map>

#define DEBUG

struct Color3 {
    unsigned char r, g, b;
};

struct Color4 {
    unsigned char r, g, b, a;
};

struct Point {
    int x,y;
    bool operator == (const Point& point) const {
        return this->x == point.x && this->y == point.y;
    }
};


struct Move {
    Point cell;
    int gain{};
    unsigned long boardHash{};

    [[nodiscard]] bool isValid() const {
        return gain != 0;
    }
    bool operator < (const Move& move) const {
        return this->gain < move.gain;
    }
};

enum GameMode : int {
    Local,
    AI,
    Online,
};

enum AiDifficulty : int {
    Random,
    Easy,
    Average,
    Hard,
};

// ai settings
extern bool showAiMove;
extern Move cachedAiMove;
extern int aiDiskMul;
extern int aiEdgeDiskMul;
extern int aiAdjacentCornerDiskMul;
extern int aiCornerDiskMul;

extern bool aiManual;
extern const char* aiDifficultyStr;
extern AiDifficulty aiDifficulty;
extern const char* aiColorStr;
extern char aiColor;
extern int aiDepth;

// eval settings
extern bool showEval;
extern int quiescenceSearchLim;
extern bool quiescenceSearchEnabled;
extern std::string currentEvalText;
extern std::unordered_map<uint64_t, Move> bestMoveNow;
extern std::unordered_map<uint64_t, Move> bestMoveNext;
// eval bar animation
extern bool enableEvalBar;
extern double evalBarAnimationRate;
extern double evalBarValueGoal;
extern double evalBarValue;

// window settings
extern bool showDebugWindow;
extern bool showGameWindow;
extern bool showWinWindow;
extern bool winWindowFocus;
extern bool gameOver;

extern int windowStartX, windowStartY;

extern int width;
extern int height;

#define CURRENT_PLAYER (char)(clientTurn ? (clientIsWhite ? 2 : 1) : (clientIsWhite ? 1 : 2))
#define NEXT_PLAYER (char)(clientTurn ? (clientIsWhite ? 1 : 2) : (clientIsWhite ? 2 : 1))

#define LOCAL_PLAYER clientIsWhite ? 2 : 1

extern GameMode gameMode;
extern const char* gameModeStr;

extern bool gameStarted;
extern bool clientTurn;
extern bool clientIsWhite;
extern int boardSize;
extern float gridLineWidth;

extern Color4 highlightPossibleColor;
extern bool highlightPossibleCells;
extern Color4 highlightModifiedColor;
extern bool highlightModifiedCells;
extern Color4 highlightAiColor;
extern bool highlightAiMove;

extern uint64_t modifiedCells;
extern std::vector<Move> currentLegalMoves;
extern bool highlighted[8][8];
extern FastBoard gameBoard;


extern GLFWwindow* glfwWindow;

#endif //REVERSI_STATE_H
