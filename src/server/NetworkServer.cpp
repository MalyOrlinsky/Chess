#include "NetworkServer.hpp"

#include <iostream>

NetworkServer::NetworkServer(int port, GameEngine engine, CommandExecutor& executor) : websocketServer(port, engine, executor) {}

void NetworkServer::start()
{
    std::cout
        << "Network server starting..."
        << std::endl;

    websocketServer.start();
}