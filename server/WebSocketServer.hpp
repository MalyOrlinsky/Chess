// #pragma once
// #include <websocketpp/config/asio_no_tls.hpp>
// #include <websocketpp/server.hpp>

// class WebSocketServer
// {
// public:

//     explicit WebSocketServer(int port);

//     void start();


// private:

//     using Server = websocketpp::server<websocketpp::config::asio>;

//     Server server;

//     int port;
// };


#pragma once

// #define ASIO_STANDALONE
// #define _WEBSOCKETPP_CPP11_STL_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

class WebSocketServer
{
public:

    explicit WebSocketServer(int port);

    void start();

private:

    using Server = websocketpp::server<websocketpp::config::asio>;

    Server server;

    int port;
};