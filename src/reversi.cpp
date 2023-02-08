#include "reversi.h"

void initReversi() {
    // generate starting disks
    board[3][3] = 1;
    board[3][4] = 2;
    board[4][3] = 2;
    board[4][4] = 1;
}

std::vector<Point> getSurroundingCoordinates(Point p) {
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

// TODO: Optimize all the following functions
// FIXME: OPTIMIZE!!!!!!
void playMove(Point move) {
    char client = clientIsWhite ? 2 : 1;
    int modified = 0;
    for (auto point : getSurroundingCoordinates(move)) {
        if (board[point.x][point.y] == 0) continue;
        // get direction vector x -> y
        int dx = move.x - point.x, dy = move.y - point.y;
        // invert the direction
        int idx = -1 * dx, idy = -1 * dy;

        int i = 1;
        int x = move.x + i * idx, y = move.y + i * idy;

        // keep count of the amount of opponent disks we've encountered because for a valid path this has to be > 0
        int opponentCount = 0;
        bool legal = false;
        std::vector<Point> opponentDisks;
        while (x > 0 && x < boardSize && y > 0 && y < boardSize) {
            // if we encounter an empty space this direction is automatically invalid
            if (board[x][y] == 0) break;
            if (board[x][y] != client) {
                opponentDisks.push_back({x,y});
                opponentCount++;
            }
            if (board[x][y] == client && opponentCount > 0) {
                legal = true;
                break;
            }
            i++;
            x = move.x + i * idx, y = move.y + i * idy;
        }

        if (!legal) continue;
        for (auto disk : opponentDisks) {
            board[disk.x][disk.y] = client;
            modified++;
        }
    }
    if (modified > 0) board[move.x][move.y] = client;
}

// FIXME: Optimize multiple usages this has to have a more efficient implementation but im retarded
bool hasOppositeDisk(Point p, const std::vector<Point>& surround) {
    char client = clientIsWhite ? 2 : 1;
    for (auto point : surround) {
        if (board[point.x][point.y] == 0) continue;
        // get direction vector x -> y
        int dx = p.x - point.x, dy = p.y - point.y;
        // invert the direction
        int idx = -1 * dx, idy = -1 * dy;

        int i = 1;
        int x = p.x + i * idx, y = p.y + i * idy;

        // keep count of the amount of opponent disks we've encountered because for a valid path this has to be > 0
        int opponentCount = 0;
        while (x > 0 && x < boardSize && y > 0 && y < boardSize) {
            // if we encounter an empty space this direction is automatically invalid
            if (board[x][y] == 0) break;
            if (board[x][y] != client) opponentCount++;
            if (board[x][y] == client && opponentCount > 0) return true;
            i++;
            x = p.x + i * idx, y = p.y + i * idy;
        }
    }
    return false;
}

bool moveIsValid(Point move) {
    // if there is already a disk here we can't place there
    if (board[move.x][move.y] != 0) return false;
    // check if there is a valid direction that can be played
    return hasOppositeDisk(move, getSurroundingCoordinates(move));
}

std::vector<Point> getPossibleMoves() {
    char client = clientIsWhite ? 2 : 1;
    std::vector<Point> moves;

    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == 0 || board[i][j] == client) continue;

            auto points = getSurroundingCoordinates({i, j});
            for (auto point : points) {
                if (!moveIsValid(point)) continue;
                moves.push_back(point);
            }
        }
    }

    return moves;
}

void highLightPossibleMoves(Shader* shader) {
    float squareSize = 1.0f / (float)boardSize;

    auto possibleMoves = getPossibleMoves();
    for (auto square : possibleMoves) {
        float x = (-1 + squareSize) + (float)square.x * squareSize*2;
        float y = (-1 + squareSize) + (float)square.y * squareSize*2;
        shader->set("centre", glm::vec2(x,y));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}