#pragma once

#include "../../client/NetworkClient.hpp"

#include <thread>
#include <chrono>

class Lobby
{
public:
    Lobby(NetworkClient& client);

    void run();

private:
    NetworkClient& client;
};