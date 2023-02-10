#include "ai.h"

Move randomMove(const std::vector<Move>& moves) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    auto start = moves.begin(), end = moves.end();
    std::uniform_int_distribution<> dis(0, (int)std::distance(start, end) - 1);
    std::advance(start, dis(gen));
    return *start;
}

Move getBestMoveRandom() {
    auto moves = Game::GetPossibleMoves(aiColor);
    if (moves.empty()) return {};
    return randomMove(moves);
}

Move getBestMoveEasy() {
    auto moves = Game::GetPossibleMoves(aiColor);
    std::sort(moves.begin(), moves.end());
    if (moves.empty()) return {};
    return moves[moves.size()-1];
}

Move Game::AI::GetBestMove() {
    switch (aiDifficulty) {
        case Random:
            return getBestMoveRandom();
        case Easy:
            return getBestMoveEasy();
        case Average:
            break;
        case Difficult:
            break;
    }
    return {};
}

void Game::AI::PlayBestMove() {
    Move m = Game::AI::GetBestMove();
    Game::PlayMove(m, aiColor);
}

SimulationContext::SimulationContext() {
    std::memcpy(this->simBoard, gameBoard, sizeof(simBoard));
}

std::vector<Move> SimulationContext::GetPositionInfo() {
    std::vector<Move> moves;
    auto boardState = Game::Board::Hash(this->simBoard);

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (this->simBoard[i][j] == 0 || this->simBoard[i][j] == aiColor) continue;

            auto points = Game::Board::GetSurroundingCoordinates({i, j});
            for (auto point : points) {
                if (this->simBoard[point.x][point.y] != 0) continue;
                Move move = Game::Board::GetValidDirectionsForCell(point, this->simBoard, aiColor);
                if (!move.isValid()) continue;
                move.boardHash = boardState;
                moves.push_back(move);
            }
        }
    }
    return moves;
}
