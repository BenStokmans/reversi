#ifndef REVERSI_MOVES_H
#define REVERSI_MOVES_H

#include "edges.h"
#include "flip.h"
#include "../uint128_t/uint128_t.h"

uint64_t hash(uint64_t p, uint64_t o);
uint64_t legalMoves(uint64_t player, uint64_t opponent);
uint64_t calcFlip(uint64_t p, uint64_t o, uint_fast8_t place);
uint64_t calcFlip(uint64_t p, uint64_t o, uint_fast8_t x, uint_fast8_t y);

#endif //REVERSI_MOVES_H
