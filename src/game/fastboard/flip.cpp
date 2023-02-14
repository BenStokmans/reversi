#include "flip.h"

// inverse direction is 7 - index
uint64_t flipPreCalc[64][8][7];

void initPreCalcPart(uint_fast8_t x, uint_fast8_t y) {
    uint_fast8_t place = y*8+x;
    uint_fast8_t di = 0;
    // allocate memory here and copy it to the cache every time instead of allocating new memory every time
    uint64_t line;
    uint_fast8_t li;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;

            line = 0;
            li = 0;
            uint_fast8_t cx = x + dx, cy = y + dy;

            while ((cx >= 0 && cx < 8) && (cy >= 0 && cy < 8)) {
                line |= 1ULL << (cy*8+cx);
                flipPreCalc[place][di][li] = line;
                cx += dx;
                cy += dy;
                li++;
            }
            di++;
        }
    }
}

void initFlipPreCalc() {
    for (uint_fast8_t y = 0; y < 8; y++) {
        for (uint_fast8_t x = 0; x < 8; x++) {
            initPreCalcPart(x, y);
        }
    }
}