#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "../src/network/Serializer.hpp"
#include "../src/commands/CommandExecutor.hpp"
#include "../src/game_engine/GameEngine.hpp"
#include "PlayerSession.hpp"
#include <vector>
class WebSocketServer
{
public:
    WebSocketServer(int port, GameEngine &engine, CommandExecutor &executor);

    void start();

private:
    Color getPlayerColor(websocketpp::connection_hdl hdl);

    using Server = websocketpp::server<websocketpp::config::asio>;

    std::vector<PlayerSession> players;

    Server server;
    int port;

    GameEngine &engine;
    CommandExecutor &executor;
};