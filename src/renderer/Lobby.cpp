#include "Lobby.hpp"

#include <iostream>

Lobby::Lobby(NetworkClient &client)
    : client(client)
{
}

void Lobby::run()
{
    std::string username;

    std::cout << "Username: ";
    std::cin >> username;

    client.sendLogin(username);

    std::cout
        << "Type PLAY or ROOM: ";

    std::string command;
    std::cin >> command;

    if (command == "PLAY")
    {
        client.sendPlay();

        std::cout
            << "Waiting for opponent..."
            << std::endl;

        while (!client.isGameStarted())
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    else if (command == "ROOM")
    {
        std::string roomName;

        std::cout << "Room name: ";
        std::cin >> roomName;

        client.sendRoom(roomName);
    }
}