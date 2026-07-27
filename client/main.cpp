#include "NetworkClient.hpp"
#include "../src/renderer/ImgRenderer.hpp"
#include "../Board.hpp"
#include "../src/renderer/Lobby.hpp"

// #include <chrono>
// #include <thread>
#include <iostream>

int main()
{
    try
    {
        ImgRenderer renderer("assets/image", "assets/board.png", ROW, COL);

        NetworkClient client;

        client.connect("localhost", 8080);

        Lobby lobby(client);

        lobby.run();

        // std::string username;

        // std::cout << "Username: ";
        // std::cin >> username;

        // client.sendLogin(username);

        // std::cout << "Press ENTER to search for a game...";
        // std::cin.ignore();
        // std::cin.get();

        // client.sendPlay();

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

        // while (!client.isGameStarted())
        // {
        //     std::this_thread::sleep_for(
        //         std::chrono::milliseconds(100));
        // }

        renderer.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cin.get();
    }

    return 0;
}