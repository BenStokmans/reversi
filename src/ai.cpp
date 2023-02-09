#include "ai.h"

Move randomMove(const std::vector<Move>& moves) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    auto start = moves.begin(), end = moves.end();
    std::uniform_int_distribution<> dis(0, (int)std::distance(start, end) - 1);
    std::advance(start, dis(gen));
    return *start;
}

Move getBestMoveRandom(char color) {
    auto moves = getPossibleMoves(color);
    return randomMove(moves);
}

Move getBestMove(AiDifficulty difficulty, char color, int depth) {
    switch (difficulty) {
        case Random:
            return getBestMoveRandom(color);
        case Easy:
            break;
        case Average:
            break;
        case Difficult:
            break;
    }
    return {};
}

