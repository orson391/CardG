#include <iostream>
#include <thread>
#include <chrono>
#include "Server.h"
#include "Client.h"

void runServer() {
    GameServer server(12345);  // Initialize the server on port 12345
    server.startServer();
}

void runClient() {
    GameClient client("127.0.0.1", 12345);  // Connect to server running on localhost at port 12345
    client.connectToServer();
}

int main() {
    // Start the server in a separate thread
    std::thread serverThread(runServer);
    std::this_thread::sleep_for(std::chrono::seconds(1));  // Give server time to start

    // Start the client in the main thread
    runClient();

    // Wait for the server thread to finish
    serverThread.join();

    return 0;
}
