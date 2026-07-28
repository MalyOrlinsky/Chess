#include "MatchmakingManager.hpp"

MatchmakingManager::MatchmakingManager(GameManager &gameManager, MatchFoundCallback callback)
    : gameManager(gameManager), matchFoundCallback(callback) {}

void MatchmakingManager::addPlayer(PlayerSession *player)
{
    queue.push_back(player);

    player->searchingGame = true;

    tryMatch();
}

void MatchmakingManager::tryMatch()
{
    if (queue.size() < 2)
        return;

    PlayerSession *player1 = queue[0];
    PlayerSession *player2 = queue[1];

    queue.erase(queue.begin(), queue.begin() + 2);

    createMatch(*player1, *player2);
}

void MatchmakingManager::createMatch(PlayerSession &white, PlayerSession &black)
{
    int gameId = createGame();

    assignPlayers(white, black, gameId);

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