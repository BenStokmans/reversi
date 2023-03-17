#include "handler.h"
#include "src/game/reversi.h"

void HandleMessage(const google::protobuf::Any &any) {
    if (any.Is<reversi::JoinGameResponse>()) return handleJoinGameResponse(any);
    if (any.Is<reversi::CreateGameResponse>()) return handleCreateGameResponse(any);
    if (any.Is<reversi::GameStart>()) return handleGameStart(any);
    if (any.Is<reversi::GameEnded>()) return handleGameGameEnded(any);
    if (any.Is<reversi::GameMove>()) return handleGameMove(any);
    if (any.Is<reversi::PlayMoveResponse>()) return handlePlayMoveResponse(any);
}

void handleCreateGameResponse(const google::protobuf::Any &any) {
    reversi::CreateGameResponse msg;
    if(!any.UnpackTo(&msg)) return;

    if (!msg.error().empty()) {
        logger.error("NETWORK", "could not create game: " + msg.error());
        onlineErrorStr = "Error: " + msg.error();
        onlineError = true;
        return;
    }
    onlineGameId = msg.game_id();
    assignedColor = msg.color();
    clientTurn = false;

    logger.info("NETWORK", "created gameId: " + std::to_string(onlineGameId));
}

void handleGameStart(const google::protobuf::Any &any) {
    reversi::GameStart msg;
    if(!any.UnpackTo(&msg)) return;
    Game::Init();

    clientTurn = assignedColor == reversi::BLACK;
    clientIsWhite = assignedColor == reversi::WHITE;
    gameStarted = true;
    logger.info("NETWORK", "game started");
}

void handleGameMove(const google::protobuf::Any &any) {
    reversi::GameMove msg;
    if(!any.UnpackTo(&msg)) return;

    Point p = {(int)msg.x(), (int)msg.y()};

    uint_fast8_t pos = p.y * 8 + p.x;
    modifiedCells = 0;
    modifiedCells = gameBoard.Play(pos);
    clientTurn = msg.pass() ? clientTurn : !clientTurn;
}

void handlePlayMoveResponse(const google::protobuf::Any &any) {
    reversi::PlayMoveResponse msg;
    if(!any.UnpackTo(&msg)) return;

    if (msg.success()) return;

    logger.error("NETWORK", "error playing move: " + msg.error());
    onlineErrorStr = "Error: " +  msg.error();
    onlineError = true;

    gameBoard = prevBoard;
    clientTurn = !clientTurn;
}

void handleJoinGameResponse(const google::protobuf::Any &any) {
    reversi::JoinGameResponse msg;
    if(!any.UnpackTo(&msg)) return;

    if (!msg.error().empty()) {
        logger.error("NETWORK", "could not join game: " + msg.error());
        onlineErrorStr = "Error: " +  msg.error();
        onlineError = true;
    }

    assignedColor = msg.color();
}

void handleGameGameEnded(const google::protobuf::Any &any) {
    reversi::GameEnded msg;
    if(!any.UnpackTo(&msg)) return;

    if (msg.winner() == reversi::Color::BLACK) {
        logger.info("NETWORK", "game ended: black won");
    } else if (msg.winner() == reversi::Color::WHITE) {
        logger.info("NETWORK", "game ended: white won");
    } else {
        logger.info("NETWORK", "game ended: draw");
    }
    Game::Init();
    gameStarted = false;
    onlineGameId = 0;

    if (msg.special_reason().empty()) return;
    onlineErrorStr = msg.special_reason();
    onlineError = true;
}
