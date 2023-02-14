#include "ai.h"

std::unordered_map<uint64_t, int> maxCache;
std::unordered_map<uint64_t, int> minCache;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
int minmax(FastBoard board, int alpha, int beta, int depth, bool max) {
    if (depth == 0) {
        return board.Eval(max);
    }

    // we have to use the flat disk count instead of the eval because it could be that weights otherwise influence the result
    if (board.GameOver()) {
        if (max) {
            if (countSetBits(board.player) > countSetBits(board.player))
                return std::numeric_limits<int>::max();
            else
                return std::numeric_limits<int>::min();
        }
        if (countSetBits(board.player) > countSetBits(board.player))
            return std::numeric_limits<int>::min();
        else
            return std::numeric_limits<int>::max();
    }

    unsigned long boardHash = board.Hash();
    if (max) {
        auto cachedMax = maxCache.find(boardHash);
        if (cachedMax != maxCache.end()) {
            return cachedMax->second;
        }

        int maxEval = std::numeric_limits<int>::min();
        uint64_t moves = board.Moves();
        for (uint_fast8_t y = 0; y < 8; y++) {
            bool b = false;
            for (uint_fast8_t x = 0; x < 8; x++) {
                uint_fast8_t place = y * 8 + x;
                uint64_t cell = 1ULL << place;
                if ((cell & moves) == 0) continue;

                FastBoard local = board.Clone();
                local.Play(place);
                int eval = minmax(local, alpha, beta, depth - 1, false);
                if (eval > maxEval)
                    maxEval = eval;
                if (eval > alpha)
                    alpha = eval;
                // prune any upcoming positions as we have found the best possibility for this branch
                if (beta <= alpha) {
                    b = true;
                    break;
                }
            }
            if (b) break;
        }
        maxCache.insert({boardHash, maxEval});
        return maxEval;
    }

    auto cachedMin = minCache.find(boardHash);
    if (cachedMin != minCache.end()) {
        return cachedMin->second;
    }

    int minEval = std::numeric_limits<int>::max();
    uint64_t moves = board.Moves();
    for (uint_fast8_t y = 0; y < 8; y++) {
        bool b = false;
        for (uint_fast8_t x = 0; x < 8; x++) {
            uint_fast8_t place = y * 8 + x;
            uint64_t cell = 1ULL << place;
            if ((cell & moves) == 0) continue;

            FastBoard local = board.Clone();
            local.Play(place);
            int eval = minmax(local, alpha, beta, depth - 1, true);
            if (eval < minEval)
                minEval = eval;
            if (eval < beta)
                beta = eval;
            // prune any upcoming positions as we have found the best possibility for this branch
            if (beta <= alpha) {
                b = true;
                break;
            }
        }
        if (b) break;
    }

    minCache.insert({boardHash, minEval});
    return minEval;
}
#pragma clang diagnostic pop

Move getMoveMinMax(SimulationContext* context, int depth) {
    int maxEval = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();

    Point bestCell{};
    FastBoard board;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (context->simBoard[x][y] == 0) continue;
            uint_fast8_t place = y * 8 + x;
            uint64_t cell = 1ULL << place;
            if (context->simBoard[x][y] == context->playerColor) {
                board.player |= cell;
                continue;
            }
            board.opponent |= cell;
        }
    }

    uint64_t moves = board.Moves();
    for (uint_fast8_t y = 0; y < 8; y++) {
        bool b = false;
        for (uint_fast8_t x = 0; x < 8; x++) {
            uint_fast8_t place = y * 8 + x;
            uint64_t cell = 1ULL << place;
            if ((cell & moves) == 0) continue;

            FastBoard local = board.Clone();
            local.Play(place);
            int eval = minmax(local, alpha, beta, depth - 1, false);
            if (eval > maxEval) {
                maxEval = eval;
                bestCell = {x, y};
            }
            if (eval > alpha)
                alpha = eval;
            // prune any upcoming positions as we have found the best possibility for this branch
            if (beta <= alpha) {
                b = true;
                break;
            }
        }
        if (b) break;
    }
    return Game::Board::GetValidDirectionsForCell(bestCell, context->simBoard, context->colorToPlay);
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