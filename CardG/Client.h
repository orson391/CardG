#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <SDL_net.h>
#include "PlayerG.h"
#include "CardG.h"

class GameClient {
public:
    GameClient(const std::string& serverAddress, int port);
    ~GameClient();
    void connectToServer();
    void sendMessage(const std::string& message);
    std::string receiveMessage();

private:
    std::string serverAddress;
    int port;
    TCPsocket clientSocket;
    IPaddress ipAddress;
};

#endif // CLIENT_H
