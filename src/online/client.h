#ifndef REVERSI_CLIENT_H
#define REVERSI_CLIENT_H


#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <thread>
#include "../logger.h"
#include "src/online/reversi.pb.h"
#include <google/protobuf/any.pb.h>

#define PORT 8080

void sendMsg(const google::protobuf::Message& message);

namespace Client {
    void Connect();
    void Disconnect();
}

#endif //REVERSI_CLIENT_H
