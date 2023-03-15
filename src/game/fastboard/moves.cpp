#include "moves.h"

// we have to initialize these like this because the compiler doesn't allow constants larger than the max value of a
// 64-bit unsigned integer
uint128_t a = (((uint128_t)0x1D15F8C153AB3B6BUL) << 64) | 0x6E6E63F10BD4FD48UL;
uint128_t b = (((uint128_t)0x06B265B3CC88EF3BUL) << 64) | 0xE56FFC797D5D431DUL;
uint128_t c = (((uint128_t)0xB8F238E50CC7CACCUL) << 64) | 0xFC27CCEEFE6D02BBUL;

// use a kind of bit mixing to create a unique enough hash of the current position
uint64_t hash(uint64_t p, uint64_t o) {
    return (uint64_t)((a * o + b * p + c) >> 64);
}

// original code from http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm
uint64_t legalMoves(const uint64_t player, const uint64_t opponent) {
    unsigned long long moves, mO;
    unsigned long long flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
    mO = opponent & 0x7e7e7e7e7e7e7e7eULL;
    flip1 = mO & (player << 1); flip7  = mO & (player << 7); flip9  = mO & (player << 9); flip8  = opponent & (player << 8);
    flip1 |= mO & (flip1 << 1);    flip7 |= mO & (flip7 << 7);    flip9 |= mO & (flip9 << 9);    flip8 |= opponent & (flip8 << 8);
    pre1 = mO & (mO << 1);         pre7 = mO & (mO << 7);         pre9 = mO & (mO << 9);         pre8 = opponent & (opponent << 8);
    flip1 |= pre1 & (flip1 << 2);  flip7 |= pre7 & (flip7 << 14); flip9 |= pre9 & (flip9 << 18); flip8 |= pre8 & (flip8 << 16);
    flip1 |= pre1 & (flip1 << 2);  flip7 |= pre7 & (flip7 << 14); flip9 |= pre9 & (flip9 << 18); flip8 |= pre8 & (flip8 << 16);
    moves = flip1 << 1;            moves |= flip7 << 7;           moves |= flip9 << 9;           moves |= flip8 << 8;
    flip1 = mO & (player >> 1); flip7  = mO & (player >> 7); flip9  = mO & (player >> 9); flip8  = opponent & (player >> 8);
    flip1 |= mO & (flip1 >> 1);    flip7 |= mO & (flip7 >> 7);    flip9 |= mO & (flip9 >> 9);    flip8 |= opponent & (flip8 >> 8);
    pre1 >>= 1;                    pre7 >>= 7;                    pre9 >>= 9;                    pre8 >>= 8;
    flip1 |= pre1 & (flip1 >> 2);  flip7 |= pre7 & (flip7 >> 14); flip9 |= pre9 & (flip9 >> 18); flip8 |= pre8 & (flip8 >> 16);
    flip1 |= pre1 & (flip1 >> 2);  flip7 |= pre7 & (flip7 >> 14); flip9 |= pre9 & (flip9 >> 18); flip8 |= pre8 & (flip8 >> 16);
    moves |= flip1 >> 1;           moves |= flip7 >> 7;           moves |= flip9 >> 9;           moves |= flip8 >> 8;
    return moves & ~(player | opponent);
}

uint64_t calcFlip(uint64_t p, uint64_t o, uint_fast8_t x, uint_fast8_t y) {
    return calcFlip(p, o, y*8+x);
}

uint64_t calcFlip(uint64_t p, uint64_t o, uint_fast8_t place) {
    uint64_t open = ~(p | o);

    uint64_t flip = 0;
    for (uint_fast8_t i = 0; i < 8; i++) {
        bool of = false;
        for (uint_fast8_t j = 0; j < 7; j++) {
            if (flipPreCalc[place][i][j] == 0) break;
            if ((open & flipPreCalc[place][i][j]) != 0) break;
            if ((o & flipPreCalc[place][i][j]) != 0 && !of) {
                of = true;
                continue;
            }
            if ((p & flipPreCalc[place][i][j]) != 0 && !of) {
                break;
            }
            if ((p & flipPreCalc[place][i][j]) != 0 && of) {
                flip |= flipPreCalc[place][i][j-1];
                break;
            }
        }
    }
    flip |= (1ULL << place);
    return flip;
}
