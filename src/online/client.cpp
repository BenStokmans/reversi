#include "client.h"

int sock = 0;

void heartbeatLoop() {
    while (sock != 0) {
        reversi::Heartbeat heartbeat{};
        Client::Send(heartbeat);

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
    HandleMessage(any);
}

void Client::Send(const google::protobuf::Message& message) {
    google::protobuf::Any any;
    if (!any.PackFrom(message)) {
        logger.error("NETWORK", "could not pack message");
        return;
    }

    size_t s = any.ByteSizeLong();
    char buffer[s];
    if (!any.SerializeToArray(buffer, (int)s)) {
        logger.error("NETWORK", "could not serialize message");
        return;
    }
    // if send succeeds return
    if (send(sock, buffer, s, 0) != -1) return;

    // otherwise do error handling
    int error_code;
    socklen_t error_code_size = sizeof(error_code);
    getsockopt(sock, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
    if (error_code != 0) {
        logger.error("NETWORK", strerror(error_code));
    }
    Client::Disconnect();
}

void receiveLoop() {
    char buffer[1024];
    while (sock != 0) {
        // read error code from previous read
        int error_code;
        socklen_t error_code_size = sizeof(error_code);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
        if (error_code != 0) {
            logger.error("NETWORK", strerror(error_code));
            break;
        }
        bzero(buffer, 1024);
        ssize_t n = read(sock, buffer, 1024);
        if (n < 0) continue;

        google::protobuf::Any any;
        any.ParseFromArray(buffer, (int)n);

        if (any.ParseFromArray(buffer,(int)n)) {
            if (any.type_url().empty()) {
                continue;
            }
            handleAnyMessage(any);
        }
    }
    logger.info("NETWORK", "disconnected from server");
}

void Client::Connect() {
    if (connected) {
        return;
    }
    struct sockaddr_in serv_addr{};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        logger.error("NETWORK","socket creation error");
        onlineErrorStr = "Error: socket creation failed";
        onlineError = true;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        logger.error("NETWORK", "invalid address/address not supported");
        onlineErrorStr = "Error: invalid address";
        onlineError = true;
        return;
    }

    if (connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) != 0) {
        logger.error("NETWORK", "connection failed");
        onlineErrorStr = "Error: connection failed";
        onlineError = true;
        sock = 0;
        return;
    }
    logger.info("NETWORK", "client successfully connected");
    connected = true;
    std::thread t(receiveLoop);
    t.detach();
}

void Client::LeaveGame() {
    if (sock == 0) {
        logger.error("NETWORK", "not connected to server");
        return;
    }
    if (onlineGameId == 0)
        return;
    reversi::LeaveGame msg{};
    Client::Send(msg);

    Game::Init();
    gameStarted = false;
    onlineGameId = 0;
}

void Client::Disconnect() {
    if (sock == 0) return;
    logger.info("NETWORK", "disconnecting client");

    // create a copy of the socket handle so the running network threads also exit
    int sCopy = sock;
    sock = 0;
    connected = false;

    int status = shutdown(sCopy, SHUT_RDWR);
    if (status != 0) {
        logger.error("NETWORK", "could not shutdown socket");
    }
    status = close(sCopy);
    if (status != 0) {
        logger.error("NETWORK", "could not close socket");
    }
}

void Client::JoinGame() {
    if (sock == 0) {
        logger.error("NETWORK", "not connected to server");
        return;
    }

    reversi::JoinGame msg{};
    try {
        onlineGameId = std::stoll(gameId);
    } catch (std::invalid_argument& e) {
        logger.error("NETWORK", "invalid game id");
        onlineErrorStr = "Error: invalid game id";
        onlineError = true;
        return;
    }
    msg.set_game_id(onlineGameId);
    msg.set_allocated_player_name(new std::string(username));
    Client::Send(msg);
}

void Client::CreateGame() {
    if (sock == 0) {
        logger.error("NETWORK", "not connected to server");
        return;
    }
    reversi::CreateGame msg{};
    msg.set_allocated_player_name(new std::string(username));
    if (onlineColor != reversi::Color::RANDOM)
        msg.set_color(onlineColor);
    Client::Send(msg);
}

void Client::PlayMove(const Point &point) {
    if (sock == 0) {
        logger.error("NETWORK", "not connected to server");
        return;
    }
    reversi::PlayMove msg{};
    msg.set_x(point.x);
    msg.set_y(point.y);
    Client::Send(msg);
}
