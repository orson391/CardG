#include <iostream>
#include <SDL_net.h>
#include <string>

class GameClient {
private:
    TCPsocket clientSocket;
    SDLNet_SocketSet socketSet;

    void sendToServer(const std::string& message) {
        int len = message.length() + 1;
        SDLNet_TCP_Send(clientSocket, message.c_str(), len);
    }

    std::string receiveFromServer() {
        char buffer[512];
        int bytesReceived = SDLNet_TCP_Recv(clientSocket, buffer, 512);
        if (bytesReceived > 0) {
            return std::string(buffer);
        }
        return "";
    }

public:
    GameClient(const std::string& serverIP, int serverPort) {
        if (SDLNet_Init() < 0) {
            std::cerr << "SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
            exit(1);
        }

        socketSet = SDLNet_AllocSocketSet(1);

        IPaddress ip;
        if (SDLNet_ResolveHost(&ip, serverIP.c_str(), serverPort) == -1) {
            std::cerr << "SDLNet_ResolveHost failed: " << SDLNet_GetError() << std::endl;
            exit(1);
        }

        clientSocket = SDLNet_TCP_Open(&ip);
        if (!clientSocket) {
            std::cerr << "SDLNet_TCP_Open failed: " << SDLNet_GetError() << std::endl;
            exit(1);
        }

        SDLNet_TCP_AddSocket(socketSet, clientSocket);
    }

    void connectToServer() {
        std::cout << "Connected to server!" << std::endl;
        sendToServer("Hello from client!");

        while (true) {
            int numReady = SDLNet_CheckSockets(socketSet, 0);
            if (numReady > 0) {
                if (SDLNet_SocketReady(clientSocket)) {
                    std::string serverMessage = receiveFromServer();
                    if (!serverMessage.empty()) {
                        std::cout << "Server says: " << serverMessage << std::endl;
                    }
                }
            }
        }
    }

    ~GameClient() {
        SDLNet_TCP_Close(clientSocket);
        SDLNet_FreeSocketSet(socketSet);
        SDLNet_Quit();
    }
};

int main(int argc, char* argv[]) {
    GameClient client("127.0.0.1", 12345); // Connecting to localhost on port 12345
    client.connectToServer();
    return 0;
}
