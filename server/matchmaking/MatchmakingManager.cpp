#include "MatchmakingManager.hpp"

MatchmakingManager::MatchmakingManager(GameManager &gameManager, MatchFoundCallback callback)
    : gameManager(gameManager), matchFoundCallback(callback) {}

void MatchmakingManager::addPlayer(PlayerSession *player)
{
    queue.push_back(player);

    std::cout
    << "ADD PLAYER "
    << player->username
    << " RATING="
    << player->rating
    << std::endl;

    player->searchingGame = true;

    tryMatch();
}

void MatchmakingManager::tryMatch()
{
    if (queue.size() < 2)
        return;

    int bestIndex = -1;
    int bestDifference = INT_MAX;

    PlayerSession *newestPlayer = queue.back();

    std::cout
    << "SEARCH MATCH FOR "
    << newestPlayer->username
    << " RATING="
    << newestPlayer->rating
    << std::endl;

    for (int i = 0; i < static_cast<int>(queue.size()) - 1; i++)
    {
        PlayerSession *candidate = queue[i];
        int difference = abs(newestPlayer->rating - candidate->rating);

        std::cout
    << "CHECK "
    << candidate->username
    << " RATING="
    << candidate->rating
    << " DIFF="
    << difference
    << std::endl;

        if (difference < bestDifference)
        {
            bestDifference = difference;
            bestIndex = i;
        }
    }

    if (bestIndex == -1)
        return;

    PlayerSession *opponent = queue[bestIndex];

    queue.erase(queue.begin() + bestIndex);
    queue.pop_back();

    createMatch(*newestPlayer, *opponent);
}

void MatchmakingManager::createMatch(PlayerSession &white, PlayerSession &black)
{
    int gameId = createGame();

    assignPlayers(white, black, gameId);

    Game &game = gameManager.getGame(gameId);

    game.setPlayers(&white, &black);

    if (matchFoundCallback)
        matchFoundCallback(white, black);
}

int MatchmakingManager::createGame()
{
    return gameManager.createGame();
}

void MatchmakingManager::assignPlayers(PlayerSession &white, PlayerSession &black, int gameId)
{
    white.gameId = gameId;
    white.color = Color::White;
    white.searchingGame = false;

    black.gameId = gameId;
    black.color = Color::Black;
    black.searchingGame = false;
}

void MatchmakingManager::startMatch(PlayerSession &player1, PlayerSession &player2)
{
    createMatch(player1, player2);
}

void MatchmakingManager::handleRoom(Room &room)
{
    if (room.users.size() == 2)
        createMatch(*room.users[0], *room.users[1]);

    else if (room.users.size() > 2)
    {
        PlayerSession *spectator = room.users.back();

        spectator->gameId = room.users[0]->gameId;
        spectator->color = Color::None;
    }
}