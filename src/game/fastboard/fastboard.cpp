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

uint64_t FastBoard::Play(uint_fast8_t x, uint_fast8_t y) {
    uint64_t flip = calcFlip(this->player, this->opponent, x, y);
    this->player |= flip;
    this->opponent &= ~flip;
    this->SwitchTurn();
    return flip;
}

uint64_t FastBoard::Play(uint_fast8_t place) {
    uint64_t flip = calcFlip(this->player, this->opponent, place);
    this->player |= flip;
    this->opponent &= ~flip;
    this->SwitchTurn();
    return flip;
}

uint64_t cornerMask = 0x8100000000000081ULL;
uint64_t edgeMask = 0x3C0081818181003CULL;
uint64_t adjCornerMask = 0x4281000000008142ULL;

int FastBoard::Eval(bool max) const {
    int32_t pEval =
            countSetBits(this->player) * aiDiskMul +
            countSetBits(this->player & edgeMask) * aiEdgeDiskMul +
            countSetBits(this->player & cornerMask) * aiCornerDiskMul -
            countSetBits(this->player & adjCornerMask) * aiAdjacentCornerDiskMul;
    int32_t oEval =
            countSetBits(this->opponent) * aiDiskMul +
            countSetBits(this->opponent & edgeMask) * aiEdgeDiskMul +
            countSetBits(this->opponent & cornerMask) * aiCornerDiskMul -
            countSetBits(this->opponent & adjCornerMask) * aiAdjacentCornerDiskMul;

    return max ? pEval - oEval : oEval - pEval;
}

bool FastBoard::GetIsCurrentPlayer(uint_fast8_t x, uint_fast8_t y) const {
    return (this->player & 1ULL << (y*8+x)) != 0;
}

bool FastBoard::CellIsOpen(uint_fast8_t x, uint_fast8_t y) {
    return (~(this->player | this->opponent) & 1ULL << (y*8+x)) != 0;
}

int FastBoard::PlayerDisks() const {
    return countSetBits(this->player);
}

int FastBoard::OpponentDisks() const {
    return countSetBits(this->opponent);
}
