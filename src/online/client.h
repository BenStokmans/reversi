#ifndef REVERSI_CLIENT_H
#define REVERSI_CLIENT_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <thread>
#include "../logger.h"
#include "reversi.pb.h"
#include <google/protobuf/any.pb.h>

#define PORT 8080

void sendMsg(const google::protobuf::Message& message);

namespace Client {
    void Connect();
    void Disconnect();
}

#endif //REVERSI_CLIENT_H
