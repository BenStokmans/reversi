#ifndef REVERSI_EDGES_H
#define REVERSI_EDGES_H

#include <cstdint>

extern uint64_t edgeTable[64];

void initEdgeTable();
uint64_t getEdges(uint64_t p, uint64_t o);

#endif
