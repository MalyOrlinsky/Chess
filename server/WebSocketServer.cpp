#include "WebSocketServer.hpp"
#include <iostream>

WebSocketServer::WebSocketServer(int port) : port(port) {}

void WebSocketServer::start()
{
    try
    {
        server.init_asio();

        server.set_message_handler(
            [this](
                websocketpp::connection_hdl hdl,
                Server::message_ptr msg)
            {
                server.send(
                    hdl,
                    msg->get_payload(),
                    msg->get_opcode()
                );
            }
        );

        server.listen(port);
        server.start_accept();

        std::cout << "WebSocket server listening on port " << port << std::endl;

        server.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "WebSocket error: " << e.what() << std::endl;
    }
}