#include "NetworkClient.hpp"

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

                case Network::MessageType::LobbyStatus:
                    handleLobbyStatus(message);
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

void NetworkClient::connect(const std::string &host, int port)
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

void NetworkClient::sendLogin(const std::string &username)
{
    if (!connected)
    {
        std::cerr
            << "Client not connected"
            << std::endl;

        return;
    }
    std::cout << "LOGIN SENT: " << username << std::endl;
    Network::Message message;

    message.type = Network::MessageType::Login;
    message.payload = username;

    std::string json = Network::Serializer::serialize(message);

    client.send(connection, json, websocketpp::frame::opcode::text);
}

void NetworkClient::sendPlay()
{
    if (!connected)
    {
        std::cerr
            << "Client not connected"
            << std::endl;

        return;
    }

    Network::Message message;

    message.type = Network::MessageType::Play;
    message.payload = "";

    std::string json = Network::Serializer::serialize(message);

    client.send(connection, json, websocketpp::frame::opcode::text);
}

GameSnapshot NetworkClient::getLatestSnapshot()
{
    std::lock_guard<std::mutex> lock(snapshotMutex);

    return latestSnapshot;
}

Color NetworkClient::getMyColor() const
{
    return myColor;
}

void NetworkClient::handleSnapshot(const std::string &data)
{
    GameSnapshot snapshot = Network::Serializer::deserializeSnapshot(data);

    std::lock_guard<std::mutex> lock(snapshotMutex);

    latestSnapshot = snapshot;
}

void NetworkClient::handlePlayerInfo(const Network::Message &message)
{
    if (message.payload == "White")
        myColor = Color::White;
    else if (message.payload == "Black")
        myColor = Color::Black;
    else
        myColor = Color::None;
}

void NetworkClient::handleLobbyStatus(const Network::Message &message)
{
    lobbyStatus = message.payload;

    if (lobbyStatus == "GameFound")
    {
        gameStarted = true;
    }

    std::cout
        << "LOBBY STATUS: "
        << lobbyStatus
        << std::endl;
}

std::string NetworkClient::getLobbyStatus() const
{
    return lobbyStatus;
}

bool NetworkClient::isGameStarted() const
{
    return gameStarted;
}

void NetworkClient::sendRoom(const std::string& roomName)
{
    if (!connected)
        return;

    Network::Message message;

    message.type = Network::MessageType::Room;
    message.payload = roomName;

    std::string json =
        Network::Serializer::serialize(message);

    client.send(
        connection,
        json,
        websocketpp::frame::opcode::text);
}