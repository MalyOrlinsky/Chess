#include "UserManager.hpp"

UserManager::UserManager(Database &database) : database(database) {}

bool UserManager::login(PlayerSession &player, const std::string &username)
{
    if (username.empty())
        return false;

    if (!database.userExists(username))
        database.createUser(username);

    player.username = username;
    player.rating = database.getUserRating(username);
    player.loggedIn = true;

    std::cout
    << "LOGIN USER="
    << player.username
    << " RATING="
    << player.rating
    << std::endl;

    return true;
}