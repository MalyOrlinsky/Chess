#include "NetworkClient.hpp"
#include "../src/renderer/ImgRenderer.hpp"
#include "../Board.hpp"
#include <thread>
#include <iostream>

int main()
{
    try
    {
        ImgRenderer renderer("assets/image", "assets/board.png", ROW, COL);

        NetworkClient client;

        client.connect(
            "localhost",
            8080);

        std::string username;

        std::cout << "Username: ";
        std::cin >> username;

        client.sendLogin(username);
        client.sendPlay();

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