#include "WebSocketServer.hpp"
#include <iostream>

WebSocketServer::WebSocketServer(
    int port,
    GameEngine &engine,
    CommandExecutor &executor)
    : port(port),
      engine(engine),
      executor(executor)
{
}

void WebSocketServer::start()
{
    try
    {
        server.init_asio();

        server.set_open_handler(
            [this](websocketpp::connection_hdl hdl)
            {
                std::cout
                    << "SERVER CLIENT CONNECTED"
                    << std::endl;

                Color color;

                if (players.empty())
                    color = Color::White;
                else if (players.size() == 1)
                    color = Color::Black;
                else
                    color = Color::None;

                players.push_back({hdl, color, ""});

                Network::Message playerMessage;

                playerMessage.type = Network::MessageType::PlayerInfo;

                if (color == Color::White)
                    playerMessage.payload = "white";
                else if (color == Color::Black)
                    playerMessage.payload = "black";
                else
                    playerMessage.payload = "spectator";

                std::string colorJson = Network::Serializer::serialize(playerMessage);

                std::cout << "SEND TO CLIENT:\n"
          << colorJson
          << std::endl;
                server.send(hdl, colorJson, websocketpp::frame::opcode::text);

                GameSnapshot snapshot = engine.snapshot();

                std::cout
                    << "SERVER INITIAL SNAPSHOT: "
                    << snapshot.rows
                    << " "
                    << snapshot.cols
                    << std::endl;

                Network::Message message;
                message.type = Network::MessageType::Snapshot;
                message.payload = Network::Serializer::serializeSnapshot(snapshot);

                std::string json = Network::Serializer::serialize(message);

                std::cout << "SEND TO CLIENT:\n"
          << json
          << std::endl;
                server.send(hdl, json, websocketpp::frame::opcode::text);
            });

        server.set_message_handler(
            [this](websocketpp::connection_hdl hdl, Server::message_ptr msg)
            {
                std::cout << "SERVER GOT MESSAGE" << std::endl;
                auto message = Network::Serializer::deserialize(msg->get_payload());

                Color color = getPlayerColor(hdl);

                if (message.type == Network::MessageType::Command)
                    executor.execute(message.payload, color);

                GameSnapshot snapshot = engine.snapshot();

                Network::Message response;
                response.type = Network::MessageType::Snapshot;
                response.payload = Network::Serializer::serializeSnapshot(snapshot);

                std::string json = Network::Serializer::serialize(response);

                std::cout << "SEND TO CLIENT:\n"
          << json
          << std::endl;
                server.send(hdl, json, websocketpp::frame::opcode::text);
            });

        server.listen(port);
        server.start_accept();

        std::cout
            << "WebSocket server listening on port "
            << port
            << std::endl;

        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr
            << "WebSocket error: "
            << e.what()
            << std::endl;
    }
}

Color WebSocketServer::getPlayerColor(websocketpp::connection_hdl hdl)
{
    for (const auto &player : players)
    {
        if (player.hdl.lock() == hdl.lock())
            return player.color;
    }

    return Color::None;
}