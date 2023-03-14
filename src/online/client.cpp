#include "client.h"

int sock = 0;

void heartbeatLoop() {
    while (sock != 0) {
        reversi::Heartbeat heartbeat{};
        sendMsg(heartbeat);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void handleConnected(const google::protobuf::Any& any)  {
    reversi::Connected msg;

    if(!any.UnpackTo(&msg)) return;
    logger.info("NETWORK", "playerId: " + std::to_string(msg.player_id()));

    std::thread t(heartbeatLoop);
    t.detach();
}

void handleAnyMessage(const google::protobuf::Any& any) {
    const std::string& type = any.type_url();
    if (type == "type.googleapis.com/reversi.Connected") return handleConnected(any);
}

void sendMsg(const google::protobuf::Message& message) {
    google::protobuf::Any any;
    if (!any.PackFrom(message)) {
        logger.error("NETWORK", "could not pack message");
        return;
    }


    size_t s = any.ByteSizeLong();
    char buffer[s];
    if (!any.SerializeToArray(buffer, s)) {
        logger.error("NETWORK", "could not serialize message");
        return;
    }
    send(sock, buffer, s, 0);
}

void receiveLoop() {
    char buffer[1024];
    while (sock != 0) {
        int error_code;
        socklen_t error_code_size = sizeof(error_code);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
        if (error_code != 0) {
            logger.error("NETWORK", strerror(error_code));
        }
        bzero(buffer, 1024);
        ssize_t n = read(sock, buffer, 1024);
        if (n < 0) continue;

        google::protobuf::Any any;
        any.ParseFromArray(buffer, n);

        if (any.ParseFromArray(buffer,n)) {
            if (any.type_url().empty()) {
                continue;
            }
            handleAnyMessage(any);
        }
    }
    close(sock);
    logger.info("NETWORK", "disconnected from server");
    sock = 0;
}

void Client::Connect() {
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(1,1), &wsa_data);
#endif

    struct sockaddr_in serv_addr{};
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        logger.error("NETWORK","socket creation error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        logger.error("NETWORK", "invalid address/address not supported");
        return;
    }

    if (int client_fd = connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) {
        logger.error("NETWORK", "connection failed");
        return;
    }
    logger.info("NETWORK", "client successfully connected");

    std::thread t(receiveLoop);
    t.detach();
}

void Client::Disconnect() {
    int status = 0;
    logger.info("NETWORK", "disconnecting client");

#ifdef _WIN32
    status = shutdown(sock, SD_BOTH);
    if (status == 0) status = closesocket(sock);
#else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0) status = close(sock);
#endif
    // TODO: handle status

#ifdef _WIN32
    WSACleanup();
#endif
}
