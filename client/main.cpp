#include "NetworkClient.hpp"
#include "../src/renderer/ImgRenderer.hpp"
#include "../Board.hpp"
#include "../src/renderer/Lobby.hpp"

#include <iostream>

int main()
{
    try
    {
        ImgRenderer renderer("assets/image", "assets/board.png", ROW, COL);

        NetworkClient client;

        client.connect("13.60.152.181", 8080);

        Lobby lobby(client);

        lobby.run();

        renderer.setCommandCallback(
            [&](const std::string &cmd)
            {
                client.sendCommand(cmd);
            });

        renderer.setSnapCallback(
            [&]()
            {
                return client.getLatestSnapshot();
            });

        renderer.setColorCallback(
            [&]()
            {
                return client.getMyColor();
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