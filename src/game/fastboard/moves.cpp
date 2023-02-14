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

uint64_t legalMoves(uint64_t p, uint64_t o) {
    uint64_t edges = getEdges(p, o);
    uint64_t open = ~(p | o);

    uint64_t out = 0;
    for (uint_fast8_t y = 0; y < 8; y++) {
        for (uint_fast8_t x = 0; x < 8; x++) {
            uint_fast8_t place = y * 8 + x;

            uint64_t cell = 1ULL << place;

            if ((edges & cell) == 0) continue;
            bool valid = false;

            for (uint_fast8_t i = 0; i < 8; i++) {
                bool dirValid = false;
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
                        dirValid = true;
                        break;
                    }
                }
                if (dirValid) {
                    valid = true;
                    break;
                }
            }
            if (!valid) continue;
            out |= cell;
            break;
        }
    }
    return out;
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
