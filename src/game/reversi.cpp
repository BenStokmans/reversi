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
    if (gameMode == GameMode::AI) {
        clientIsWhite = aiColor == 1;
        if (clientIsWhite)
            clientTurn = false;
    }

    // zero out all board cells
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            gameBoard[i][j] = 0;
        }
    }

    // set starting disks
    gameBoard[3][3] = 1;
    gameBoard[3][4] = 2;
    gameBoard[4][3] = 2;
    gameBoard[4][4] = 1;
}

void Game::PlayMove(const Move& move, char color) {
    if (!move.isValid()) return;
    if (!gameStarted)
        gameStarted = true;

    // clear modified cells
    modifiedCells.clear();

    for (auto direction : move.directions) {
        int dx = direction.x, dy = direction.y;
        int x = move.cell.x + dx, y = move.cell.y + dy;

        while (x > -1 && x < boardSize && y > 0 && y < boardSize) {
            // if we encounter an empty space this direction is automatically invalid
            if (gameBoard[x][y] == 0) break;
            if (gameBoard[x][y] == color) break;
            if (gameBoard[x][y] != color) {
                gameBoard[x][y] = color;
                modifiedCells.push_back({x, y});
            }
            x += dx, y += dy;
        }
    }
    gameBoard[move.cell.x][move.cell.y] = color;
    modifiedCells.push_back(move.cell);
    clientTurn = !clientTurn;

    currentLegalMoves.clear();
    auto moves = Game::GetPossibleMoves(CURRENT_PLAYER);
    if (moves.empty()) {
        gameOver = true;
        return;
    }
}

Move Game::GetMove(Point point) {
    // if there is already a disk here we can't place there
    if (gameBoard[point.x][point.y] != 0) return {};
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

    auto boardState = Board::State(gameBoard);
    std::vector<Move> moves;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (gameBoard[i][j] == 0 || gameBoard[i][j] == color) continue;

            auto points = Board::GetSurroundingCoordinates({i, j});
            for (auto point : points) {
                if (gameBoard[point.x][point.y] != 0) continue;
                Move move = Board::GetValidDirectionsForCell(point, gameBoard, color);
                if (!move.isValid()) continue;
                move.boardState = boardState;
                moves.push_back(move);
            }
        }
    }
    currentLegalMoves = moves;

    return moves;
}