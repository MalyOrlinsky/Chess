#include "GameEngine.hpp"
#include "NetworkServer.hpp"


int main()
{
    NetworkServer server(8080);

    GameEngine engine;

    engine.loadBoard("board.txt");


    server.start();


    while(true)
    {
        engine.update();

        auto snapshot = engine.snapshot();

        server.broadcast(snapshot);
    }

    return 0;
}