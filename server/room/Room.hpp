#pragma once

#include "../PlayerSession.hpp"

#include <string>
#include <vector>

struct Room
{
    std::string name;

    int gameId = -1;

    std::vector<PlayerSession*> users;
};