#include "game/GameManager.hpp"
#include "game/GameLoop.hpp"
#include "network/WebSocketServer.hpp"
#include "database/Database.hpp"

#include <iostream>

int main()
{
    Database database("kungfu-postgres", "5432", "kungfu_chess", "chess", "chess");

    std::cout << "Database connected: "
              << database.isConnected()
              << std::endl;

    database.initialize();

    GameManager gameManager(database);

    int gameId = gameManager.createGame();
    gameManager.loadBoard(gameId, "board.txt");

    WebSocketServer server(8080, gameManager, database);

    GameLoop loop(
        gameManager,
        [&]()
        {
            server.sendAllSnapshots();
        });

    loop.start();
    server.start();

    return 0;
}