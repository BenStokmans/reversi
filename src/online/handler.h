#ifndef REVERSI_HANDLER_H
#define REVERSI_HANDLER_H

#include <google/protobuf/any.pb.h>
#include "src/online/reversi.pb.h"
#include "../logger.h"
#include "../state.h"

void handleCreateGameResponse(const google::protobuf::Any& any);
void handleGameStart(const google::protobuf::Any& any);
void handleGameMove(const google::protobuf::Any& any);
void handlePlayMoveResponse(const google::protobuf::Any& any);
void handleJoinGameResponse(const google::protobuf::Any& any);
void handleGameGameEnded(const google::protobuf::Any& any);

void HandleMessage(const google::protobuf::Any& any);

#endif //REVERSI_HANDLER_H
