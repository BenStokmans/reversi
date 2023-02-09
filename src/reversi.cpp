#include "reversi.h"

void initReversi() {
    // reset game over popup
    gameOver = false;
    winWindowFocus = true;

    // clear data from previous game
    currentLegalMoves.clear();
    clientIsWhite = false;
    clientTurn = true;

    // zero out all board squares
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j] = 0;
        }
    }

    // set starting disks
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

void playMove(const Move& move) {
    char client = clientIsWhite ? 2 : 1;
    for (auto direction : move.directions) {
        int dx = direction.x, dy = direction.y;
        int x = move.square.x + dx, y = move.square.y + dy;

        while (x > -1 && x < boardSize && y > 0 && y < boardSize) {
            // if we encounter an empty space this direction is automatically invalid
            if (board[x][y] == 0) break;
            if (board[x][y] == client) {
                break;
            }
            if (board[x][y] != client) {
                board[x][y] = client;
            }
            x += dx, y += dy;
        }
    }
    // if (modified > 0) board[move.x][move.y] = client;
    board[move.square.x][move.square.y] = client;
    currentLegalMoves.clear();
    auto moves = getPossibleMoves(NEXT_PLAYER);
    if (moves.empty()) {
        gameOver = true;
        return;
    }
    if (!clientTurn) currentLegalMoves.clear();
}

Move getValidDirectionsForSquare(Point square, char color) {
    std::vector<Point> directions;
    for (auto point : getSurroundingCoordinates(square)) {
        if (board[point.x][point.y] == 0) continue;
        // get direction vector x -> y
        int dx = square.x - point.x, dy = square.y - point.y;
        // invert the direction
        int idx = -1 * dx, idy = -1 * dy;
        int x = square.x + idx, y = square.y + idy;

        // keep count of the amount of opponent disks we've encountered because for a valid path this has to be > 0
        int opponentCount = 0;
        while (x > -1 && x < boardSize && y > -1 && y < boardSize) {
            // if we encounter an empty space this direction is automatically invalid
            if (board[x][y] == 0) break;
            if (board[x][y] != color) opponentCount++;
            if (board[x][y] == color && opponentCount == 0) break;
            if (board[x][y] == color && opponentCount > 0) {
                directions.push_back({idx, idy});
                break;
            }
            x += idx, y += idy;
        }
    }
    auto move = Move{square, directions};
    if (!directions.empty()) {
        currentLegalMoves.push_back(move);
    }
    return {square, directions};
}

Move getMove(Point point) {
    // if there is already a disk here we can't place there
    if (board[point.x][point.y] != 0) return {};
    // check if the move is in current legal moves
    for (Move move : currentLegalMoves) {
        if (point.x == move.square.x && point.y == move.square.y) {
            return move;
        }
    }
    return {};
}

std::vector<Move> getPossibleMoves(char color) {
    // if we already calculated all the legal moves for this turn return those
    if (!currentLegalMoves.empty()) {
        return currentLegalMoves;
    }

    std::vector<Move> moves;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == 0 || board[i][j] == color) continue;

            auto points = getSurroundingCoordinates({i, j});
            for (auto point : points) {
                if (board[point.x][point.y] != 0) continue;
                Move move = getValidDirectionsForSquare(point, color);
                if (move.directions.empty()) continue;
                moves.push_back(move);
            }
        }
    }

    return moves;
}

void highLightPossibleMoves(Shader* shader) {
    float squareSize = 1.0f / (float)boardSize;

    auto possibleMoves = getPossibleMoves(LOCAL_PLAYER);
    for (const auto& move : possibleMoves) {
        float x = (-1 + squareSize) + (float)move.square.x * squareSize*2;
        float y = (-1 + squareSize) + (float)move.square.y * squareSize*2;
        shader->set("centre", glm::vec2(x,y));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}