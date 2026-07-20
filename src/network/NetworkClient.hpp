#pragma once

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <string>

class NetworkClient
{
public:
    NetworkClient();

    void connect(
        const std::string &host,
        int port);

    void sendCommand(
        const std::string &command);

private:
    using Client = websocketpp::client<websocketpp::config::asio_client>;

    Client client;

    websocketpp::connection_hdl connection;

    bool connected = false;
};