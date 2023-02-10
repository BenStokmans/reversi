#ifndef REVERSI_AI_H
#define REVERSI_AI_H

#include <random>
#include "reversi.h"
#include "board.h"

class SimulationContext {
public:
    SimulationContext(char startColor, char board[8][8]);
    explicit SimulationContext(SimulationContext* context);
    std::vector<Move> GetMovesForPosition();
    void PlayMove(const Move& move);
    SimulationContext NewPositionFromMove(const Move& move);

    int Eval();
    bool GameOver();
    void SwitchTurn() {
        colorToPlay = colorToPlay == 1 ? 2 : 1;
    }
    char playerColor{};
    char colorToPlay{};
    char simBoard[8][8]{};
};

namespace Game::AI {
    Move GetBestMove();
    void PlayBestMove();

}

#endif //REVERSI_AI_H
