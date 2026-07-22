#include "WebSocketServer.hpp"
#include <iostream>

WebSocketServer::WebSocketServer(
    int port,
    GameManager &gameManager)
    : port(port),
      gameManager(gameManager)
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

                PlayerSession session;

                session.hdl = hdl;
                session.username = "";
                session.color = Color::None;
                session.gameId = -1;

                players.push_back(session);

                Network::Message playerMessage;

                playerMessage.type = Network::MessageType::PlayerInfo;
                playerMessage.payload = "lobby";

                std::string colorJson = Network::Serializer::serialize(playerMessage);

                std::cout << "SEND TO CLIENT:\n"
                          << colorJson
                          << std::endl;
                server.send(hdl, colorJson, websocketpp::frame::opcode::text);

                PlayerSession &player = getPlayer(hdl);

                if (player.gameId != -1)
                {
                    Game &game = gameManager.getGame(player.gameId);

                    GameSnapshot snapshot = game.snapshot();
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
                }
            });

        server.set_message_handler(
            [this](websocketpp::connection_hdl hdl, Server::message_ptr msg)
            {
                std::cout << "SERVER GOT MESSAGE" << std::endl;

                auto message = Network::Serializer::deserialize(msg->get_payload());

                if (message.type == Network::MessageType::Login)
                {
                    PlayerSession *existing =
                        findPlayerByUsername(message.payload);

                    if (existing)
                    {
                        existing->hdl = hdl;
                        existing->loggedIn = true;
                    }
                    else
                    {
                        PlayerSession session;

                        session.hdl = hdl;
                        session.username = message.payload;
                        session.color = Color::None;
                        session.gameId = -1;
                        session.loggedIn = true;

                        players.push_back(session);
                    }

                    return;
                }

                PlayerSession &player = getPlayer(hdl);

                if (message.type == Network::MessageType::Play)
                {
                    if (!player.searchingGame && player.gameId == -1)
                    {
                        player.searchingGame = true;
                        matchmakingQueue.push_back(hdl);

                        tryMatchmaking();
                    }

                    return;
                }

                if (player.gameId == -1)
                    return;

                Game &game = gameManager.getGame(player.gameId);
                if (message.type == Network::MessageType::Command)
                    game.executeCommand(message.payload, player.color);

                GameSnapshot snapshot = game.snapshot();

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

PlayerSession &WebSocketServer::getPlayer(websocketpp::connection_hdl hdl)
{
    for (auto &player : players)
    {
        if (player.hdl.lock() == hdl.lock())
            return player;
    }

    throw std::runtime_error("Player not found");
}

void WebSocketServer::tryMatchmaking()
{
    if (matchmakingQueue.size() < 2)
        return;

    websocketpp::connection_hdl hdl1 = matchmakingQueue[0];
    websocketpp::connection_hdl hdl2 = matchmakingQueue[1];

    PlayerSession &player1 = getPlayer(hdl1);
    PlayerSession &player2 = getPlayer(hdl2);

    int gameId = gameManager.createGame();

    player1.gameId = gameId;
    player1.color = Color::White;
    player1.searchingGame = false;

    player2.gameId = gameId;
    player2.color = Color::Black;
    player2.searchingGame = false;

    matchmakingQueue.erase(matchmakingQueue.begin(),
                           matchmakingQueue.begin() + 2);

    std::cout
        << "MATCH CREATED gameId="
        << gameId
        << std::endl;
}

PlayerSession *WebSocketServer::findPlayerByUsername(
    const std::string &username)
{
    for (auto &player : players)
    {
        if (player.username == username)
            return &player;
    }

    return nullptr;
}