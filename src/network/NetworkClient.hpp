#pragma once

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include "../game_engine/GameSnapshot.hpp"
#include "Message.hpp"

#include <string>

class NetworkClient
{
public:
    NetworkClient();

    void connect(const std::string &host, int port);
    void sendCommand(const std::string &command);
    GameSnapshot getLatestSnapshot();
    Color getMyColor() const;

private:

    void handleSnapshot(const std::string& data);
    void handlePlayerInfo(const Network::Message& message);

    using Client = websocketpp::client<websocketpp::config::asio_client>;

    Client client;
    GameSnapshot latestSnapshot;
    Color myColor;

    websocketpp::connection_hdl connection;

    bool connected = false;
};