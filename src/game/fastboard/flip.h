#ifndef REVERSI_FLIP_H
#define REVERSI_FLIP_H

#include <cstdint>

extern uint64_t flipPreCalc[64][8][7];
void initFlipPreCalc();
uint64_t getLinesFromPoint(uint_fast8_t x, uint_fast8_t y);
#endif //REVERSI_FLIP_H
