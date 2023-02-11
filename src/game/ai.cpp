#include "ai.h"

// maybe use a BST instead of an unordered map
// TODO: add optimised depth caching
std::unordered_map<unsigned long, int> maxCache;
std::unordered_map<unsigned long, int> minCache;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
// TODO: Implement quiescence search in the future.
int minmax(SimulationContext* context, int alpha, int beta, int depth, bool max) {
    if (depth == 0) {
        return context->Eval();
    }
    // TODO: find out if max and min here are too strong as weight
    if (context->GameOver()) {
        return context->Eval() > 0 ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();
    }

    unsigned long boardHash = Game::Board::Hash(context->simBoard);
    if (max) {
        auto cachedMax = maxCache.find(boardHash);
        if (cachedMax != maxCache.end()) {
            return cachedMax->second;
        }

        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : context->GetMovesForPosition()) {
            auto moveCtx = context->NewPositionFromMove(move);
            int eval = minmax(&moveCtx, alpha, beta, depth - 1, false);
            if (eval > maxEval)
                maxEval = eval;
            if (eval > alpha)
                alpha = eval;
            // prune any upcoming positions as we have found the best possibility for this branch
            if (beta <= alpha)
                break;
        }
        maxCache.insert({boardHash, maxEval});
        return maxEval;
    }

    auto cachedMin = minCache.find(boardHash);
    if (cachedMin != minCache.end()) {
        return cachedMin->second;
    }

    int minEval = std::numeric_limits<int>::max();
    for (const auto& move : context->GetMovesForPosition()) {
        auto moveCtx = context->NewPositionFromMove(move);
        int eval = minmax(&moveCtx, alpha, beta, depth - 1, true);
        if (eval < minEval)
            minEval = eval;
        if (eval < beta)
            beta = eval;
        // same here
        if (beta <= alpha)
            break;
    }
    minCache.insert({boardHash, minEval});
    return minEval;
}
#pragma clang diagnostic pop

Move getMoveMinMax(SimulationContext* context, int depth) {
    int maxEval = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    Move bestMove;
    for (const auto& move : context->GetMovesForPosition()) {
        auto moveCtx = context->NewPositionFromMove(move);
        int eval = minmax(&moveCtx, alpha, beta, depth - 1, false);
        if (eval > maxEval) {
            maxEval = eval;
            bestMove = move;
        }
        if (eval > alpha)
            alpha = eval;
        if (beta <= alpha)
            break;
    }
    return bestMove;
}

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

Move getBestMoveAverage() {
    auto moves = Game::GetPossibleMoves(aiColor);
    std::sort(moves.begin(), moves.end());
    if (moves.empty()) return {};

    std::reverse(moves.begin(), moves.end());
    for (auto move : moves) {
        if ((move.cell.x == 0 && move.cell.y == 0) || (move.cell.x == boardSize-1 && move.cell.y == boardSize-1))
            return move;
        if (move.cell.x == 0 || move.cell.x == boardSize-1 || move.cell.y == 0 || move.cell.y == boardSize-1)
            return move;
    }
    return moves[0];
}

Move getBestMoveHard() {
    auto context = SimulationContext(aiColor, gameBoard);
    return getMoveMinMax(&context, aiDepth);
}

Move Game::AI::GetBestMove() {
    switch (aiDifficulty) {
        case Random:
            return getBestMoveRandom();
        case Easy:
            return getBestMoveEasy();
        case Average:
            return getBestMoveAverage();
        case Hard:
            return getBestMoveHard();
    }
    return {};
}

void Game::AI::PlayBestMove() {
    Move m = Game::AI::GetBestMove();
    Game::PlayMove(m, aiColor);
}

SimulationContext::SimulationContext(char startColor, char board[8][8]) {
    std::memcpy(this->simBoard, board, sizeof(simBoard));
    this->playerColor = startColor;
    this->colorToPlay = startColor;
}

SimulationContext::SimulationContext(SimulationContext *context) {
    std::memcpy(this->simBoard, context->simBoard, sizeof(simBoard));
    this->playerColor = context->playerColor;
    this->colorToPlay = context->colorToPlay;
}

std::vector<Move> SimulationContext::GetMovesForPosition() {
    std::vector<Move> moves;
    auto boardState = Game::Board::Hash(this->simBoard);

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (this->simBoard[i][j] == 0 || this->simBoard[i][j] == colorToPlay) continue;

            auto points = Game::Board::GetSurroundingCoordinates({i, j});
            for (auto point : points) {
                if (this->simBoard[point.x][point.y] != 0) continue;
                Move move = Game::Board::GetValidDirectionsForCell(point, this->simBoard, colorToPlay);
                if (!move.isValid()) continue;
                move.boardHash = boardState;
                moves.push_back(move);
            }
        }
    }
    return moves;
}

bool SimulationContext::GameOver() {
    return this->GetMovesForPosition().empty();
}

SimulationContext SimulationContext::NewPositionFromMove(const Move& move) {
    SimulationContext newContext(this);
    newContext.PlayMove(move);
    return newContext;
}

void SimulationContext::PlayMove(const Move& move) {
    Game::Board::PlayMove(move, this->colorToPlay, this->simBoard);
    this->SwitchTurn();
}

int SimulationContext::Eval() {
    int whiteDiskEval = 0, blackDiskEval = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            char side = this->simBoard[i][j];
            int mul = aiDiskMul;
            // favor edge cells
            if (i == 0 || i == boardSize-1 || j == 0 || j == boardSize-1)
                mul = aiEdgeDiskMul;
            // favor corner cells over edge cells
            if ((i == 0 && j == 0) || (i == boardSize-1 && j == boardSize-1))
                mul = aiCornerDiskMul;
            // prioritise not blocking player by playing next to corners
            if (((i == 0 || i == boardSize-1) && (j == 1 || j == boardSize-2)) ||
                ((j == 0 || j == boardSize-1) && (i == 1 || i == boardSize-2)) ||
                ((i == 1 || i == boardSize-2) && (j == 1 || j == boardSize-2)))
                mul = aiAdjacentCornerDiskMul;

            if (side == 1)
                blackDiskEval += mul;
            if (side == 2)
                whiteDiskEval += mul;
        }
    }
    return playerColor == 1 ? blackDiskEval - whiteDiskEval : whiteDiskEval - blackDiskEval;
}
