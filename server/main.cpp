#include "GameManager.hpp"
#include "GameLoop.hpp"
#include "WebSocketServer.hpp"

int main()
{
    GameManager gameManager;

    int gameId = gameManager.createGame();
    gameManager.loadBoard(gameId, "board.txt");

    WebSocketServer server(8080, gameManager);

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