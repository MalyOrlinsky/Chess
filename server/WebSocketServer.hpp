#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "../src/network/Serializer.hpp"
#include "GameManager.hpp"
#include "PlayerSession.hpp"

#include <vector>
#include <stdexcept>

class WebSocketServer
{
public:
    WebSocketServer(int port, GameManager& gameManager);
    
    void start();

private:

    PlayerSession& getPlayer(websocketpp::connection_hdl hdl);
    PlayerSession* findPlayerByUsername(const std::string& username);
    void tryMatchmaking();

    using Server = websocketpp::server<websocketpp::config::asio>;

    std::vector<PlayerSession> players;
    std::vector<websocketpp::connection_hdl> matchmakingQueue;

    Server server;
    int port;

    GameManager& gameManager;
};