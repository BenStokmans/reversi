#include "reversi.h"
#include "ai.h"

void Game::Init() {
    // reset game over popup
    gameOver = false;
    gameStarted = false;
    winWindowFocus = true;

    // clear data from previous game
    modifiedCells.clear();
    currentLegalMoves.clear();
    clientIsWhite = false;
    clientTurn = true;
    if (aiEnabled) {
        clientIsWhite = aiColor == 1;
        if (clientIsWhite)
            clientTurn = false;
    }

    // zero out all board cells
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

void Game::PlayMove(const Move& move) {
    char color = CURRENT_PLAYER;
    // clear modified cells
    modifiedCells.clear();

    for (auto direction : move.directions) {
        int dx = direction.x, dy = direction.y;
        int x = move.cell.x + dx, y = move.cell.y + dy;

        while (x > -1 && x < boardSize && y > 0 && y < boardSize) {
            // if we encounter an empty space this direction is automatically invalid
            if (board[x][y] == 0) break;
            if (board[x][y] == color) break;
            if (board[x][y] != color) {
                board[x][y] = color;
                modifiedCells.push_back({x, y});
            }
            x += dx, y += dy;
        }
    }
    if (!move.directions.empty()) {
        board[move.cell.x][move.cell.y] = color;
        modifiedCells.push_back(move.cell);
    }

    currentLegalMoves.clear();
    auto moves = Game::GetPossibleMoves(NEXT_PLAYER);
    if (moves.empty()) {
        gameOver = true;
        return;
    }
}

Move getValidDirectionsForCell(Point cell, char color) {
    std::vector<Point> directions;

    for (auto point : getSurroundingCoordinates(cell)) {
        if (board[point.x][point.y] == 0) continue;
        // get direction vector x -> y
        int dx = cell.x - point.x, dy = cell.y - point.y;
        // invert the direction
        int idx = -1 * dx, idy = -1 * dy;
        int x = cell.x + idx, y = cell.y + idy;

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
    auto move = Move{cell, directions};
    if (!directions.empty()) {
        currentLegalMoves.push_back(move);
    }
    return {cell, directions};
}

Move Game::GetMove(Point point) {
    // if there is already a disk here we can't place there
    if (board[point.x][point.y] != 0) return {};
    // check if the move is in current legal moves
    for (Move move : currentLegalMoves) {
        if (point.x == move.cell.x && point.y == move.cell.y) {
            return move;
        }
    }
    return {};
}

std::vector<Move> Game::GetPossibleMoves(char color) {
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
                Move move = getValidDirectionsForCell(point, color);
                if (move.directions.empty()) continue;
                moves.push_back(move);
            }
        }
    }

    return moves;
}