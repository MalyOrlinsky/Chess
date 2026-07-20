#include "NetworkServer.hpp"

#include <iostream>

NetworkServer::NetworkServer(int port)
    :
    port(port),
    websocketServer(port)
{
}

void NetworkServer::start()
{
    std::cout
        << "Network server starting..."
        << std::endl;

    websocketServer.start();
}