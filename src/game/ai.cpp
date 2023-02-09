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
    auto moves = Game::GetPossibleMoves(color);
    return randomMove(moves);
}

Move Game::AI::GetBestMove(AiDifficulty difficulty, char color, int depth) {
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

void Game::AI::PlayBestMove(AiDifficulty difficulty, char color, int depth) {
    Move m = Game::AI::GetBestMove(aiDifficulty, aiColor, aiDepth);
    Game::PlayMove(m);
    clientTurn = true;
}

