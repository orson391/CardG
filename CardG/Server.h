#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <SDL_net.h>
#include "PlayerG.h"
#include "CardG.h"

class GameServer {
public:
    GameServer(int port);
    ~GameServer();
    void startServer();
    void handleClientConnection(TCPsocket clientSocket);

private:
    int port;
    TCPsocket serverSocket;
    IPaddress ipAddress;
    SDLNet_SocketSet socketSet;
};

#endif // SERVER_H
