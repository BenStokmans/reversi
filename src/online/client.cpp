#include "client.h"

SOCKET sock = 0;

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

    // send(sock, buf, sizeof(buf), 0);
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
