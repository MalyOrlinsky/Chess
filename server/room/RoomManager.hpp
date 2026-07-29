#pragma once

#include "Room.hpp"
#include "../matchmaking/MatchmakingManager.hpp"

#include <string>
#include <vector>

class RoomManager
{
public:
    Room *findRoom(const std::string &name);
    Room &createRoom(const std::string &name);

    void addUser(Room& room, PlayerSession* player);
    Room *joinRoom(const std::string &name, PlayerSession *player, MatchmakingManager& matchmakingManager);

    bool hasEnoughPlayers(const Room& room) const;

private:

    std::vector<Room> rooms;
};