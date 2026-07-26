#include "WebSocketServer.hpp"
#include <iostream>

WebSocketServer::WebSocketServer(int port, GameManager &gameManager)
    : port(port), gameManager(gameManager) {}

void WebSocketServer::start()
{
    try
    {
        server.init_asio();

        server.set_open_handler(
            [this](websocketpp::connection_hdl hdl)
            {
                onOpen(hdl);
            });

        server.set_close_handler(
            [this](websocketpp::connection_hdl hdl)
            {
                onClose(hdl);
            });

        server.set_message_handler(
            [this](websocketpp::connection_hdl hdl, Server::message_ptr msg)
            {
                onMessage(hdl, msg);
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

void WebSocketServer::onOpen(websocketpp::connection_hdl hdl)
{
    std::cout
        << "SERVER CLIENT CONNECTED"
        << std::endl;

    PlayerSession session;

    session.id = nextPlayerId++;
    session.hdl = hdl;
    session.username = "";
    session.color = Color::None;
    session.gameId = -1;

    players.push_back(session);

    std::cout
    << "PLAYER CONNECTED id="
    << session.id
    << std::endl;

    sendLobby(hdl);

    PlayerSession &player = getPlayer(hdl);

    if (player.gameId != -1)
        sendSnapshot(hdl);
}

void WebSocketServer::onClose(websocketpp::connection_hdl hdl)
{
    // TODO
}

void WebSocketServer::onMessage(websocketpp::connection_hdl hdl, Server::message_ptr msg)
{
    std::cout
        << "SERVER GOT MESSAGE" << msg->get_payload()
        << std::endl;

    auto message = Network::Serializer::deserialize(msg->get_payload());

    if (message.type == Network::MessageType::Login)
    {
        handleLogin(hdl, message);
        return;
    }

    if (message.type == Network::MessageType::Play)
    {
        handlePlay(hdl);
        return;
    }

    if (message.type == Network::MessageType::Command)
    {
        handleCommand(hdl, message);
        return;
    }
}

void WebSocketServer::handleLogin(websocketpp::connection_hdl hdl, const Network::Message &message)
{
    PlayerSession &player = getPlayer(hdl);

    player.username = message.payload;
    player.loggedIn = true;

    std::cout
        << "LOGIN: "
        << player.username
        << std::endl;
}

// void WebSocketServer::handleLogin(websocketpp::connection_hdl hdl, const Network::Message &message)
// {
//     PlayerSession *existing = findPlayerByUsername(message.payload);

//     if (existing)
//     {
//         existing->hdl = hdl;
//         existing->loggedIn = true;
//     }
//     else
//     {
//         PlayerSession session;

//         session.hdl = hdl;
//         session.username = message.payload;
//         session.color = Color::None;
//         session.gameId = -1;
//         session.loggedIn = true;

//         players.push_back(session);
//     }
// }

void WebSocketServer::handlePlay(websocketpp::connection_hdl hdl)
{
    PlayerSession &player = getPlayer(hdl);

    if (!player.searchingGame && player.gameId == -1)
    {
        player.searchingGame = true;

        matchmakingQueue.push_back(&player);

        tryMatchmaking();
    }
}

void WebSocketServer::handleCommand(websocketpp::connection_hdl hdl, const Network::Message &message)
{
    PlayerSession &player = getPlayer(hdl);

    if (player.gameId == -1)
        return;

    Game &game = gameManager.getGame(player.gameId);

    game.executeCommand(message.payload, player.color);

    sendGameSnapshot(player.gameId);
}

void WebSocketServer::tryMatchmaking()
{
    if (matchmakingQueue.size() < 2)
        return;

    PlayerSession *player1 = matchmakingQueue[0];
    PlayerSession *player2 = matchmakingQueue[1];

    matchmakingQueue.erase(matchmakingQueue.begin(), matchmakingQueue.begin() + 2);

    createMatch(*player1, *player2);
}

void WebSocketServer::createMatch(PlayerSession &white, PlayerSession &black)
{
    int gameId = gameManager.createGame();

    white.gameId = gameId;
    white.color = Color::White;
    white.searchingGame = false;

    black.gameId = gameId;
    black.color = Color::Black;
    black.searchingGame = false;

    std::cout
        << "MATCH CREATED gameId="
        << gameId
        << std::endl;
    std::cout << "BEFORE WHITE INFO" << std::endl;

    sendPlayerInfo(white.hdl, Color::White);
    std::cout << "BEFORE BLACK INFO" << std::endl;

    sendPlayerInfo(black.hdl, Color::Black);
    std::cout << "BEFORE WHITE SNAPSHOT" << std::endl;

    sendSnapshot(white.hdl);
    std::cout << "BEFORE BLACK SNAPSHPT" << std::endl;

    sendSnapshot(black.hdl);
    std::cout << "AFTER ALL" << std::endl;
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

PlayerSession *WebSocketServer::findPlayerByUsername(const std::string &username)
{
    for (auto &player : players)
    {
        if (player.username == username)
            return &player;
    }

    return nullptr;
}

void WebSocketServer::sendLobby(websocketpp::connection_hdl hdl)
{
    Network::Message message;

    message.type = Network::MessageType::PlayerInfo;
    message.payload = "lobby";

    std::string json = Network::Serializer::serialize(message);

    std::cout
        << "SEND TO CLIENT:\n"
        << json
        << std::endl;

    websocketpp::lib::error_code ec;

    server.send(hdl, json, websocketpp::frame::opcode::text, ec);

    if (ec)
    {
        std::cout
            << "SEND LOBBY ERROR: "
            << ec.message()
            << std::endl;
    }
}

// void WebSocketServer::sendPlayerInfo(websocketpp::connection_hdl hdl, Color color)
// {
//     Network::Message message;
//     message.type = Network::MessageType::PlayerInfo;

//     if (color == Color::White)
//         message.payload = "White";

//     else if (color == Color::Black)
//         message.payload = "Black";

//     else
//         message.payload = "None";

//     std::string json = Network::Serializer::serialize(message);

//     std::cout
//         << "SEND PLAYER INFO:\n"
//         << json
//         << std::endl;

//     websocketpp::lib::error_code ec;

//     server.send(hdl, json, websocketpp::frame::opcode::text, ec);

//     if (ec)
//     {
//         std::cout
//             << "SEND ERROR: "
//             << ec.message()
//             << std::endl;
//     }
// }

void WebSocketServer::sendPlayerInfo(websocketpp::connection_hdl hdl, Color color)
{
    std::cout << "ENTER sendPlayerInfo" << std::endl;

    Network::Message message;
    message.type = Network::MessageType::PlayerInfo;

    if (color == Color::White)
        message.payload = "White";
    else if (color == Color::Black)
        message.payload = "Black";
    else
        message.payload = "None";

    std::string json = Network::Serializer::serialize(message);

    std::cout << "BEFORE SEND" << std::endl;

    websocketpp::lib::error_code ec;

    server.send(
        hdl,
        json,
        websocketpp::frame::opcode::text,
        ec);

    std::cout << "AFTER SEND" << std::endl;

    if (ec)
    {
        std::cout
            << "SEND ERROR: "
            << ec.message()
            << std::endl;
    }
}

void WebSocketServer::sendSnapshot(websocketpp::connection_hdl hdl)
{
    PlayerSession &player = getPlayer(hdl);

    if (player.gameId == -1)
        return;

    Game &game = gameManager.getGame(player.gameId);

    sendSnapshot(hdl, game.snapshot());
}

void WebSocketServer::sendSnapshot(websocketpp::connection_hdl hdl, const GameSnapshot &snapshot)
{
    Network::Message message;

    message.type = Network::MessageType::Snapshot;
    message.payload = Network::Serializer::serializeSnapshot(snapshot);

    std::string json = Network::Serializer::serialize(message);

    std::cout
        << "SEND TO CLIENT:\n"
        << json
        << std::endl;

    websocketpp::lib::error_code ec;

    server.send(hdl, json, websocketpp::frame::opcode::text, ec);

    if (ec)
    {
        std::cout
            << "SEND SNAPSHOT ERROR: "
            << ec.message()
            << std::endl;
    }
}

void WebSocketServer::sendGameSnapshot(int gameId)
{
    std::cout 
    << "SEND GAME SNAPSHOT game="
    << gameId
    << std::endl;

    for(auto &player : players)
    {
        if(player.gameId == gameId)
        {
            sendSnapshot(player.hdl);
        }
    }
}

void WebSocketServer::sendAllSnapshots()
{
    for(auto& player : players)
    {
        if(player.gameId == -1)
            continue;

        sendSnapshot(player.hdl);
    }
}