#include "ai.h"

std::unordered_map<uint64_t, int_fast8_t> maxCache;
std::mutex maxMutex;
std::unordered_map<uint64_t, int_fast8_t> minCache;
std::mutex minMutex;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
int_fast8_t minmax(FastBoard board, int_fast8_t alpha, int_fast8_t beta, int depth, bool max) {
    if (depth == 0 || board.GameOver()) {
        return board.Eval(max);
    }
    if (board.Moves() == 0) {
        board.SwitchTurn();
        max = !max;
    }

    unsigned long boardHash = board.Hash();
    if (max) {
        auto cachedMax = maxCache.find(boardHash);
        if (cachedMax != maxCache.end()) {
            return cachedMax->second;
        }

        int_fast8_t maxEval = std::numeric_limits<int_fast8_t>::min();
        uint64_t moves = board.Moves();
        for (uint_fast8_t y = 0; y < 8; y++) {
            bool b = false;
            for (uint_fast8_t x = 0; x < 8; x++) {
                uint_fast8_t place = y * 8 + x;
                uint64_t cell = 1ULL << place;
                if ((cell & moves) == 0) continue;

                FastBoard local = board.Clone();
                local.Play(place);
                int_fast8_t eval = minmax(local, alpha, beta, depth - 1, false);
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
        std::lock_guard<std::mutex> guard(maxMutex);
        maxCache.insert({boardHash, maxEval});
        return maxEval;
    }

    auto cachedMin = minCache.find(boardHash);
    if (cachedMin != minCache.end()) {
        return cachedMin->second;
    }

    int_fast8_t minEval = std::numeric_limits<int_fast8_t>::max();
    uint64_t moves = board.Moves();
    for (uint_fast8_t y = 0; y < 8; y++) {
        bool b = false;
        for (uint_fast8_t x = 0; x < 8; x++) {
            uint_fast8_t place = y * 8 + x;
            uint64_t cell = 1ULL << place;
            if ((cell & moves) == 0) continue;

            FastBoard local = board.Clone();
            local.Play(place);
            int_fast8_t eval = minmax(local, alpha, beta, depth - 1, true);
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
    std::lock_guard<std::mutex> guard(minMutex);
    minCache.insert({boardHash, minEval});
    return minEval;
}

int_fast8_t gameEndMinMax(FastBoard board, int depth, bool max) {
    if (board.GameOver()) {
        return depth;
    }

    if (depth == 0) {
        return board.Eval(max);
    }

    if (max) {
        int_fast8_t maxEval = std::numeric_limits<int_fast8_t>::min();
        uint64_t moves = board.Moves();
        for (uint_fast8_t y = 0; y < 8; y++) {
            for (uint_fast8_t x = 0; x < 8; x++) {
                uint_fast8_t place = y * 8 + x;
                uint64_t cell = 1ULL << place;
                if ((cell & moves) == 0) continue;

                FastBoard local = board.Clone();
                local.Play(place);
                int_fast8_t eval = gameEndMinMax(local, depth - 1, false);
                if (eval > maxEval)
                    maxEval = eval;
            }
        }
        return maxEval;
    }

    int_fast8_t minEval = std::numeric_limits<int_fast8_t>::max();
    uint64_t moves = board.Moves();
    for (uint_fast8_t y = 0; y < 8; y++) {
        for (uint_fast8_t x = 0; x < 8; x++) {
            uint_fast8_t place = y * 8 + x;
            uint64_t cell = 1ULL << place;
            if ((cell & moves) == 0) continue;

            FastBoard local = board.Clone();
            local.Play(place);
            int_fast8_t eval = gameEndMinMax(local, depth - 1, true);
            if (eval < minEval)
                minEval = eval;
        }
    }

    return minEval;
}

#pragma clang diagnostic pop

Move getMoveMinMax(FastBoard gameState, int depth) {
    int_fast8_t maxEval = std::numeric_limits<int_fast8_t>::min();
    int_fast8_t alpha = std::numeric_limits<int_fast8_t>::min();
    int_fast8_t beta = std::numeric_limits<int_fast8_t>::max();

    Point bestCell{};
    FastBoard board = gameState.Clone();

    uint64_t moves = board.Moves();

    for (uint_fast8_t y = 0; y < 8; y++) {
        bool b = false;
        for (uint_fast8_t x = 0; x < 8; x++) {
            uint_fast8_t place = y * 8 + x;
            uint64_t cell = 1ULL << place;
            if ((cell & moves) == 0) continue;

            FastBoard local = board.Clone();
            local.Play(place);

            int_fast8_t eval = minmax(local, alpha, beta, depth - 1, false);
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
    return Move{bestCell};
}

std::vector<Move> getPossibleMoves() {
    std::vector<Move> possibleMoves;
    auto moves = gameBoard.Moves();
    if (moves == 0) return {};

    auto hash = gameBoard.Hash();
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            uint_fast8_t place = j*8+i;
            if ((moves & 1ULL << place) == 0) continue;
            auto c = gameBoard.Clone();
            int gain = c.PlayerDisks();
            c.Play(place);
            gain = c.OpponentDisks() - gain;

            possibleMoves.emplace_back(Move{Point{i, j}, gain,hash});
        }
    }
    return possibleMoves;
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
    std::vector<Move> possibleMoves = getPossibleMoves();
    return randomMove(possibleMoves);
}

Move getBestMoveEasy() {
    std::vector<Move> possibleMoves = getPossibleMoves();

    std::sort(possibleMoves.begin(), possibleMoves.end());
    if (possibleMoves.empty()) return {};
    return possibleMoves[possibleMoves.size()-1];
}

Move getBestMoveAverage() {
    std::vector<Move> possibleMoves = getPossibleMoves();

    std::sort(possibleMoves.begin(), possibleMoves.end());
    if (possibleMoves.empty()) return {};

    std::reverse(possibleMoves.begin(), possibleMoves.end());
    for (auto move : possibleMoves) {
        if ((move.cell.x == 0 && move.cell.y == 0) || (move.cell.x == boardSize-1 && move.cell.y == boardSize-1))
            return move;
        if (move.cell.x == 0 || move.cell.x == boardSize-1 || move.cell.y == 0 || move.cell.y == boardSize-1)
            return move;
    }
    return possibleMoves[0];
}

Move getBestMoveHard() {
    auto cachedMove = bestMoveNow.find(gameBoard.Hash());
    if (cachedMove != bestMoveNow.end()) {
        auto m = cachedMove->second;
        uint_fast8_t place = m.cell.y * 8 + m.cell.x;
        if ((gameBoard.Moves() & 1ULL << place) != 0) {
            return m;
        }
    }
    return getMoveMinMax(gameBoard, aiDepth);
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

int_fast8_t Game::AI::GetEval(int depth) {
    int_fast8_t alpha = std::numeric_limits<int_fast8_t>::min();
    int_fast8_t beta = std::numeric_limits<int_fast8_t>::max();

    return minmax(gameBoard, alpha, beta, depth, CURRENT_PLAYER == 1);
}

int_fast8_t Game::AI::GetEndGame(int depth) {
    return gameEndMinMax(gameBoard, depth, true);
}

void Game::AI::PlayBestMove() {
    modifiedCells = 0;
    Move m = Game::AI::GetBestMove();
    modifiedCells = gameBoard.Play(m.cell.x, m.cell.y);
    while (gameBoard.Moves() == 0 && !gameBoard.GameOver()) {
        m = Game::AI::GetBestMove();
        modifiedCells |= gameBoard.Play(m.cell.x, m.cell.y);
        clientTurn = !clientTurn;
    }
    clientTurn = !clientTurn;
}
