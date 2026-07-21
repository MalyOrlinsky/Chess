#include "NetworkClient.hpp"

#include "Message.hpp"
#include "Serializer.hpp"

#include <thread>
#include <stdexcept>

#include <iostream>

NetworkClient::NetworkClient()
{
    client.init_asio();

    client.set_open_handler(
        [&](websocketpp::connection_hdl hdl)
        {
            connection = hdl;
            connected = true;

            std::cout
                << "WebSocket client connected"
                << std::endl;
        });
    client.set_message_handler(
        [&](websocketpp::connection_hdl, Client::message_ptr msg)
        {
            try
            {
                auto message = Network::Serializer::deserialize(msg->get_payload());

                switch (message.type)
                {
                case Network::MessageType::Snapshot:
                    handleSnapshot(message.payload);
                    break;

                case Network::MessageType::PlayerInfo:
                    handlePlayerInfo(message);
                    break;
                default:
                    break;
                }
            }
            catch (const std::exception &e)
            {
                std::cout
                    << "CLIENT ERROR: "
                    << e.what()
                    << std::endl;
            }
        });
}

void NetworkClient::connect(
    const std::string &host,
    int port)
{
    std::string uri = "ws://" + host + ":" + std::to_string(port);

    websocketpp::lib::error_code ec;

    auto con = client.get_connection(uri, ec);

    if (ec)
        throw std::runtime_error(ec.message());

    client.connect(con);

    std::thread(
        [&]()
        {
            client.run();
        })
        .detach();
}

void NetworkClient::sendCommand(const std::string &command)
{
    if (!connected)
    {
        std::cerr
            << "Client not connected"
            << std::endl;

        return;
    }

    Network::Message message;
    message.type = Network::MessageType::Command;
    message.payload = command;

    std::string json = Network::Serializer::serialize(message);

    client.send(connection, json, websocketpp::frame::opcode::text);
}

GameSnapshot NetworkClient::getLatestSnapshot()
{
    return latestSnapshot;
}

Color NetworkClient::getMyColor() const
{
    return myColor;
}

void NetworkClient::handleSnapshot(const std::string &data)
{
    latestSnapshot = Network::Serializer::deserializeSnapshot(data);
}

void NetworkClient::handlePlayerInfo(const Network::Message &message)
{
    if (message.payload == "white")
        myColor = Color::White;
    else if (message.payload == "black")
        myColor = Color::Black;
    else
        myColor = Color::None;
}