#include "fastboard.h"
#include "../search.h"

void AIEnv::Init() {
    logger.info("AI", "initializing engine environment");

    initEdgeTable();
    initFlipPreCalc();
    if (quiescenceSearchEnabled)
        Game::AI::StartSearch();
    if (enableEvalBar)
        Game::AI::StartEvalBarAnimation();
}

uint_fast8_t countSetBits(uint64_t n) {
    n = n - ((n >> 1) & 0x5555555555555555ULL);
    n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
    return (uint_fast8_t)(((n + (n >> 4)) & 0xF0F0F0F0F0F0F0FULL) * 0x101010101010101ULL >> 56);
}

uint64_t FastBoard::Play(uint_fast8_t place) {
    uint64_t flip = calcFlip(this->player, this->opponent, place);
    this->player |= flip;
    this->opponent &= ~flip;
    this->SwitchTurn();
    return flip;
}

uint64_t FastBoard::Play(uint_fast8_t x, uint_fast8_t y) {
    return this->Play(y*8+x);
}

int_fast8_t evalTable[8][8] = {
        {20, -3, 11, 8, 8, 11, -3, 20},
        {-3, -7, -4, 1, 1, -4, -7, -3},
        {11, -4, 2, 2, 2, 2, -4, 11},
        {8, 1, 2, -3, -3, 2, 1, 8},
        {8, 1, 2, -3, -3, 2, 1, 8},
        {11, -4, 2, 2, 2, 2, -4, 11},
        {-3, -7, -4, 1, 1, -4, -7, -3},
        {20, -3, 11, 8, 8, 11, -3, 20},
};
int_fast8_t xSurround[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int_fast8_t ySurround[] = {0, 1, 1, 1, 0, -1, -1, -1};

// adapted/inspired from https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
int_fast8_t dynHeuristicEval(FastBoard board)  {
    uint64_t legalPlayerMoves = legalMoves(board.player, board.opponent);
    if (countSetBits(legalPlayerMoves) == 0) {
        return board.PlayerDisks() > board.OpponentDisks() ? 100 : -100;
    }

    double diskH, cornerH, cornerCloseH, moveH, edgeDiskH, cellH = 0;

    // disk difference, disks and edge disks
    uint_fast8_t playerDisks = 0, opponentDisks = 0;
    uint_fast8_t playerEdgeDisks = 0, opponentEdgeDisks = 0;
    for (uint_fast8_t i = 0; i < 8; i++) {
        for (uint_fast8_t j = 0; j < 8; j++) {
            if (board.CellIsOpen(i, j)) continue;

            bool currentPlayer = board.GetIsCurrentPlayer(i, j);
            if (currentPlayer) {
                cellH += evalTable[i][j];
                playerDisks++;
            } else {
                cellH -= evalTable[i][j];
                opponentDisks++;
            }

            for (uint_fast8_t k = 0; k < 8; k++) {
                uint_fast8_t x = i + xSurround[k];
                uint_fast8_t y = j + ySurround[k];
                if (x >= 0 && x < 8 && y >= 0 && y < 8 && board.CellIsOpen(x, y)) {
                    currentPlayer ? playerEdgeDisks++ : opponentEdgeDisks++;
                    break;
                }
            }
        }
    }

    // make sure each heuristic value sits between -100 and 100
    if(playerDisks > opponentDisks)
        diskH = (100.0 * playerDisks) / (playerDisks + opponentDisks);
    else if(playerDisks < opponentDisks)
        diskH = -(100.0 * opponentDisks) / (playerDisks + opponentDisks);
    else diskH = 0;

    if(playerEdgeDisks > opponentEdgeDisks)
        edgeDiskH = (100.0 * playerEdgeDisks) / (playerEdgeDisks + opponentEdgeDisks);
    else if(playerEdgeDisks < opponentEdgeDisks)
        edgeDiskH = -(100.0 * opponentEdgeDisks) / (playerEdgeDisks + opponentEdgeDisks);
    else edgeDiskH = 0;

    // corner occupancy
    // corner indexes 0, 7, 56 and 64
    // 0x8100000000000081 = 0x1 | 0x1 << 7 | 0x1 << 56 | 0x1 << 64
    uint64_t cornerMask = 0x8100000000000081ULL;
    uint_fast8_t playerCornerDisks = countSetBits(board.player & cornerMask);
    uint_fast8_t opponentCornerDisks = countSetBits(board.opponent & cornerMask);
    cornerH = 25 * (playerCornerDisks - opponentCornerDisks);

    // corner closeness
    // indexes: left bottom=(1,8,9) - right bottom=(6,14,15) - left top=(48,49,57) - right top=(54,55,62)
    // 0x42C300000000C342 =
    // 0x1 << 1 | 0x1 << 8 | 0x1 << 9 |
    // 0x1 << 6 | 0x1 << 14 | 0x1 << 15 |
    // 0x1 << 48 | 0x1 << 49 | 0x1 << 57 |
    // 0x1 << 54 | 0x1 << 55 | 0x1 << 62

    uint64_t cornerClosenessMask = 0x42C300000000C342ULL;
    // left bottom indexes 1, 8 and 9
    // 0xFFFFFCFC = ~(0x1 << 1 | 0x1 << 8 | 0x1 << 9)
    if(!board.CellIsOpen(0, 0)) cornerClosenessMask &= 0xFFFFFFFFFFFCFCULL;
    // right bottom indexes 6, 14 and 15
    // 0xFFFF3FBF = ~(0x1 << 6 | 0x1 << 14 | 0x1 << 15)
    if(!board.CellIsOpen(7, 0)) cornerClosenessMask &= 0xFFFFFFFFFF3FBFULL;
    // left top indexes 48, 49 and 57
    // 0xFDFCFFFFFFFFFFFF = ~(0x1 << 48 | 0x1 << 49 | 0x1 << 57)
    if(!board.CellIsOpen(7, 0)) cornerClosenessMask &= 0xFDFCFFFFFFFFFFFFULL;
    // right top indexes 54, 55 and 62
    // 0xBF3FFFFFFFFFFFFF = ~(0x1 << 54 | 0x1 << 55 | 0x1 << 62)
    if(!board.CellIsOpen(7, 7)) cornerClosenessMask &= 0XBF3FFFFFFFFFFFFFULL;

    uint_fast8_t playerCornerCloseDisks = countSetBits(board.player & cornerClosenessMask);
    uint_fast8_t opponentCornerCloseDisks = countSetBits(board.opponent & cornerClosenessMask);

    cornerCloseH = -12.5 * (playerCornerCloseDisks - opponentCornerCloseDisks);
    cornerCloseH /= 3;

    // mobility
    uint_fast8_t playerMoves = countSetBits(legalMoves(board.player, board.opponent));
    uint_fast8_t opponentMoves = countSetBits(legalMoves(board.opponent, board.player));
    if(playerMoves > opponentMoves)
        moveH = (100.0 * playerMoves) / (playerMoves + opponentMoves);
    else if(playerMoves < opponentMoves)
        moveH = -(100.0 * opponentMoves) / (playerMoves + opponentMoves);
    else moveH = 0;

    // final weighted score
    // 0.01 + 0.59 + 0.29 + 0.05 + 0.05 + 0.01 = 1
    // 1 + 59 + 29 + 5 + 5 + 1 = 100

    // 0.15 + 0.25 + 0.25 + 0.1 + 0.1 + 0.15 = 1
    double score = (15 * diskH) + (25 * cornerH) + (25 * cornerCloseH) + (10 * moveH) + (10 * edgeDiskH) + (15 * cellH);
    auto s = (int_fast8_t)(score / 100);
    return s;
}

int_fast8_t FastBoard::Eval(bool maxPlayer) const {
    int_fast8_t eval = dynHeuristicEval(*this);

    // flip eval
    if (!maxPlayer) eval *= -1;
    return eval;
}

bool FastBoard::GetIsCurrentPlayer(uint_fast8_t x, uint_fast8_t y) const {
    return (this->player & 1ULL << (y*8+x)) != 0;
}

bool FastBoard::GetIsOpponentPlayer(uint_fast8_t x, uint_fast8_t y) const {
    return (this->opponent & 1ULL << (y*8+x)) != 0;
}

bool FastBoard::CellIsOpen(uint_fast8_t x, uint_fast8_t y) const {
    return (~(this->player | this->opponent) & 1ULL << (y*8+x)) != 0;
}

int FastBoard::PlayerDisks() const {
    return countSetBits(this->player);
}

int FastBoard::OpponentDisks() const {
    return countSetBits(this->opponent);
}
