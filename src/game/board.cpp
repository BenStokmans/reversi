#include "board.h"

std::vector<Point> Game::Board::GetSurroundingCoordinates(Point p) {
    std::vector<Point> points;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            if (p.x + dx < 0 || p.x + dx > boardSize - 1) continue;
            if (p.y + dy < 0 || p.y + dy > boardSize - 1) continue;
            points.push_back({p.x + dx, p.y + dy});
        }
    }
    return points;
}

Move Game::Board::GetValidDirectionsForCell(Point cell, char board[8][8], char color) {
    Move move = {cell};

    for (auto point : Board::GetSurroundingCoordinates(cell)) {
        if (board[point.x][point.y] == 0) continue;
        // get direction vector x -> y
        int dx = cell.x - point.x, dy = cell.y - point.y;
        // invert the direction
        int idx = -1 * dx, idy = -1 * dy;
        int x = cell.x + idx, y = cell.y + idy;

        // keep count of the amount of opponent disks we've encountered because for a valid path this has to be > 0
        int opponentCount = 0;
        int valid = false;
        while (x > -1 && x < boardSize && y > -1 && y < boardSize) {
            // if we encounter an empty space this direction is automatically invalid
            if (board[x][y] == 0) break;
            if (board[x][y] != color) opponentCount++;
            if (board[x][y] == color && opponentCount == 0) break;
            if (board[x][y] == color && opponentCount > 0) {
                valid = true;
                break;
            }
            x += idx, y += idy;
        }
        if (!valid) continue;
        move.directions.push_back({idx, idy});
        move.gain += opponentCount;
    }
    // if the move is valid also count the cell the new disk will occupy
    if (move.gain > 0) move.gain++;
    return move;
}

// TODO: Check if this also works on windows
__int128 Game::Board::State(char board[8][8]) {
    long mask = 0;
    long value = 0;

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            char val = board[i][j];
            int index = i * boardSize + j;

            // set mask bit if there is a disk on the square
            mask ^= ((val == 0 ? 0 : 1) << index);
            // set hash bit for the color of the disk
            value ^= ((val == 1 ? 0 : 1) << index);
        }
    }
    __int128 state = 0;
    state ^= ((__int128)mask) << 64;
    state ^= value;

    return state;
}

// we have to initialize these like this because the compiler doesn't allow constants larger than the max value of a
// 64-bit unsigned integer
__int128 a = (((__int128)0x1D15F8C153AB3B6BUL) << 64) | 0x6E6E63F10BD4FD48UL;
__int128 b = (((__int128)0x06B265B3CC88EF3BUL) << 64) | 0xE56FFC797D5D431DUL;
__int128 c = (((__int128)0xB8F238E50CC7CACCUL) << 64) | 0xFC27CCEEFE6D02BBUL;

long Game::Board::Hash(char board[8][8]) {
    __int128 state = State(board);
    auto low = (long)state;
    auto high = (long)(state >> 64);
    return (long)((a * low + b * high + c) >> 64);
}
