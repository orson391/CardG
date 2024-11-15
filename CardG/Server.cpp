#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL_net.h>
#include "PlayerG.h"
#include "CardG.h"

class GameServer {
private:
    SDLNet_SocketSet socketSet;
    TCPsocket serverSocket;
    std::vector<TCPsocket> clients;

    void sendToClient(TCPsocket client, const std::string& message) {
        int len = message.length() + 1;
        SDLNet_TCP_Send(client, message.c_str(), len);
    }

public:
    GameServer(int port) {
        if (SDLNet_Init() < 0) {
            std::cerr << "SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
            exit(1);
        }

        socketSet = SDLNet_AllocSocketSet(1);
        serverSocket = SDLNet_TCP_Open(&SDLNet_ResolveHost(nullptr, "localhost", port));

        if (!serverSocket) {
            std::cerr << "SDLNet_TCP_Open failed: " << SDLNet_GetError() << std::endl;
            exit(1);
        }

        SDLNet_TCP_AddSocket(socketSet, serverSocket);
    }

    void acceptClients() {
        while (true) {
            int numReady = SDLNet_CheckSockets(socketSet, 0);
            if (numReady > 0) {
                if (SDLNet_SocketReady(serverSocket)) {
                    TCPsocket newClient = SDLNet_TCP_Accept(serverSocket);
                    if (newClient) {
                        std::cout << "New client connected!" << std::endl;
                        SDLNet_TCP_AddSocket(socketSet, newClient);
                        clients.push_back(newClient);
                        sendToClient(newClient, "Welcome to the card game server!");
                    }
                }
                for (auto& client : clients) {
                    if (SDLNet_SocketReady(client)) {
                        char buffer[512];
                        int bytesReceived = SDLNet_TCP_Recv(client, buffer, 512);
                        if (bytesReceived <= 0) {
                            std::cout << "Client disconnected!" << std::endl;
                            SDLNet_TCP_DelSocket(socketSet, client);
                            SDLNet_TCP_Close(client);
                        }
                        else {
                            std::string receivedMessage(buffer);
                            std::cout << "Received: " << receivedMessage << std::endl;

                            // Echo back the message (For testing)
                            sendToClient(client, "Echo: " + receivedMessage);
                        }
                    }
                }
            }
        }
    }

    ~GameServer() {
        for (auto& client : clients) {
            SDLNet_TCP_Close(client);
        }
        SDLNet_TCP_Close(serverSocket);
        SDLNet_FreeSocketSet(socketSet);
        SDLNet_Quit();
    }
};

int main(int argc, char* argv[]) {
    GameServer server(12345);
    server.acceptClients();
    return 0;
}
