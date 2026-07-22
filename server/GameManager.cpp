#include "GameManager.hpp"

GameManager::GameManager()
{
}

int GameManager::createGame()
{
    int gameId = nextGameId++;

    games.emplace(gameId, std::make_unique<Game>());

    return gameId;
}

void GameManager::executeCommand(
    int gameId,
    const std::string& command,
    Color playerColor)
{
    Game* game = findGame(gameId);

    if (game == nullptr)
        return;

    game->executeCommand(command, playerColor);
}

GameSnapshot GameManager::snapshot(int gameId) const
{
    const Game* game = findGame(gameId);

    if (game == nullptr)
        return GameSnapshot();

    return game->snapshot();
}

void GameManager::loadBoard(
    int gameId,
    const std::string& path)
{
    Game* game = findGame(gameId);

    if (game == nullptr)
        return;

    game->loadBoard(path);
}

void GameManager::update(int deltaMs)
{
    for (auto& [id, game] : games)
    {
        game->update(deltaMs);
    }
}

Game& GameManager::getGame(int gameId)
{
    return getGameImpl(*this, gameId);
}

const Game& GameManager::getGame(int gameId) const
{
    return getGameImpl(*this, gameId);
}

void GameManager::removeGame(int gameId)
{
    games.erase(gameId);
}

Game* GameManager::findGame(int gameId)
{
    return findGameImpl(*this, gameId);
}

const Game* GameManager::findGame(int gameId) const
{
    return findGameImpl(*this, gameId);
}