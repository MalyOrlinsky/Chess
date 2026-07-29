#pragma once

#include "../PlayerSession.hpp"
#include "../database/Database.hpp"
#include "User.hpp"

#include <string>
#include <iostream>

class UserManager
{
public:
    UserManager(Database &database);

    bool login(PlayerSession &player, const std::string &username);

private:
    Database &database;
};