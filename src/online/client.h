#ifndef REVERSI_CLIENT_H
#define REVERSI_CLIENT_H


#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <thread>
#include "handler.h"
#include "src/game/reversi.h"

#define PORT 8080

namespace Client {
    void CreateGame();
    void JoinGame();
    void Send(const google::protobuf::Message& message);
    void PlayMove(const Point& point);
    void Connect();
    void Disconnect();
    void LeaveGame();
}

#endif //REVERSI_CLIENT_H
