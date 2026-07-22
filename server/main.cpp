#include "GameManager.hpp"
#include "GameLoop.hpp"
#include "WebSocketServer.hpp"

int main()
{
    GameManager gameManager;

    int gameId = gameManager.createGame();
    gameManager.loadBoard(gameId, "board.txt");

    GameLoop loop(gameManager);
    loop.start();

    WebSocketServer server(8080, gameManager);
    server.start();

    return 0;
}