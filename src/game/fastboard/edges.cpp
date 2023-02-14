#include "edges.h"

uint64_t edgeTable[64];

uint64_t edgeForCell(uint_fast8_t x, uint_fast8_t y) {
    uint_fast8_t place = y*8+x;
    // 1110000010100000111 = edge mask
    uint_fast8_t maskPlace = place - 9;
    uint64_t edge;
    if (maskPlace < 0) {
        edge = 0x70507ULL >> (maskPlace*-1);
    } else {
        edge = 0x70507ULL << maskPlace;
    }
    // 11111110 rep = 0xFEFEFEFEFEFEFEFEULL
    // 01111111 rep = 0x7F7F7F7F7F7F7F7FULL
    if (x == 0) edge &= 0x7F7F7F7F7F7F7F7FULL;
    if (x == 7) edge &= 0xFEFEFEFEFEFEFEFEULL;
    return edge;
}

void initEdgeTable() {
    for (uint_fast8_t y = 0; y < 8; y++) {
        for (uint_fast8_t x = 0; x < 8; x++) {
            uint_fast8_t place = y*8+x;
            edgeTable[place] = edgeForCell(x, y);
        }
    }
}

uint64_t getEdges(uint64_t p, uint64_t o) {
    uint64_t out = 0;
    for (uint_fast8_t y = 0; y < 8; y++) {
        for (uint_fast8_t x = 0; x < 8; x++) {
            uint_fast8_t place = y * 8 + x;
            uint64_t cell = 1ULL << place;
            if ((o & cell) == 0) continue;
            out |= edgeTable[place];
        }
    }
    out &= ~(p | o);
    return out;
}
