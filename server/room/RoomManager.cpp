#include "RoomManager.hpp"

Room *RoomManager::findRoom(const std::string &name)
{
    for (auto &room : rooms)
    {
        if (room.name == name)
            return &room;
    }

    return nullptr;
}

Room &RoomManager::createRoom(const std::string &name)
{
    Room room;
    room.name = name;

    rooms.push_back(room);

    return rooms.back();
}

void RoomManager::addUser(Room &room, PlayerSession *player)
{
    room.users.push_back(player);
}

Room *RoomManager::joinRoom(const std::string &name, PlayerSession *player, MatchmakingManager &matchmakingManager)
{
    Room *room = findRoom(name);

    if (room == nullptr)
        room = &createRoom(name);

    addUser(*room, player);

    matchmakingManager.handleRoom(*room);

    return room;
}

bool RoomManager::hasEnoughPlayers(const Room &room) const
{
    return room.users.size() >= 2;
}