#include "../src/game_engine/GameEngine.hpp"
#include "../src/commands/CommandExecutor.hpp"
#include "GameLoop.hpp"
#include "WebSocketServer.hpp"

int main()
{
    GameEngine engine;
    engine.loadBoard("board.txt");

    GameLoop loop(engine);
    loop.start();

    CommandExecutor executor(engine);

    WebSocketServer server(8080, engine, executor);

    server.start();

    return 0;
}
