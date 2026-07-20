#include "src/game_engine/GameEngine.hpp"
#include "src/renderer/ImgRenderer.hpp"
#include "server/NetworkServer.hpp"
#include "src/network/NetworkClient.hpp"
#include "src/commands/CommandExecutor.hpp"
#include <thread>
#include <iostream>

int main()
{
    try
    {
        GameEngine engine;

        engine.loadBoard("board.txt");

        ImgRenderer renderer(
            "assets/pieces2",
            "assets/board.png",
            engine.rows(),
            engine.cols());

        CommandExecutor executor(engine);

        NetworkServer server(8080, executor);

        std::thread serverThread(
            [&]()
            {
                server.start();
            });

        // renderer.setCommandCallback(
        //     [&](const std::string &cmd)
        //     {
        //         executor.execute(cmd);
        //     });

        NetworkClient client;

        client.connect(
            "localhost",
            8080);

        renderer.setCommandCallback(
            [&](const std::string &cmd)
            {
                client.sendCommand(cmd);
            });

        renderer.setSnapCallback(
            [&]()
            {
                return engine.snapshot();
            });

        renderer.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cin.get();
    }

    return 0;
}