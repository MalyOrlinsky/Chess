#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "../src/network/Serializer.hpp"
#include "GameManager.hpp"
#include "PlayerSession.hpp"

#include <list>
#include <vector>
#include <stdexcept>

class WebSocketServer
{
public:
    WebSocketServer(int port, GameManager &gameManager);

    void start();

    void sendAllSnapshots();

private:
    using Server = websocketpp::server<websocketpp::config::asio>;

    void onOpen(websocketpp::connection_hdl hdl);
    void onClose(websocketpp::connection_hdl hdl);
    void onMessage(websocketpp::connection_hdl hdl, Server::message_ptr msg);

    void handleLogin(websocketpp::connection_hdl hdl, const Network::Message &message);
    void handlePlay(websocketpp::connection_hdl hdl);
    void handleCommand(websocketpp::connection_hdl hdl, const Network::Message &message);

    void tryMatchmaking();
    void createMatch(PlayerSession &white, PlayerSession &black);

    PlayerSession &getPlayer(websocketpp::connection_hdl hdl);
    PlayerSession *findPlayerByUsername(const std::string &username);

    void sendLobby(websocketpp::connection_hdl hdl);
    void sendPlayerInfo(websocketpp::connection_hdl hdl, Color color);
    void sendGameSnapshot(int gameId);
    void sendSnapshot(websocketpp::connection_hdl hdl);
    void sendSnapshot(websocketpp::connection_hdl hdl, const GameSnapshot &snapshot);

private:
    std::list<PlayerSession> players;
    std::vector<PlayerSession *> matchmakingQueue;

    Server server;

    int port;

    int nextPlayerId = 1;

    GameManager &gameManager;
};