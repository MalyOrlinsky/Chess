#include "GameManager.hpp"

GameManager::GameManager(Database &database)
    : database(database)
{
}

int GameManager::createGame()
{
    int gameId = nextGameId++;

    games.emplace(gameId, std::make_unique<Game>());

    games[gameId]->loadBoard("board.txt");

    return gameId;
}

void GameManager::executeCommand(int gameId, const std::string &command, Color playerColor)
{
    std::lock_guard<std::mutex> lock(mutex);

    Game *game = findGame(gameId);

    if (game == nullptr)
        return;

    game->executeCommand(command, playerColor);
}

GameSnapshot GameManager::snapshot(int gameId) const
{
    std::lock_guard<std::mutex> lock(mutex);

    const Game *game = findGame(gameId);

    if (game == nullptr)
        return {};

    return game->snapshot();
}

void GameManager::loadBoard(
    int gameId,
    const std::string &path)
{
    Game *game = findGame(gameId);

    if (game == nullptr)
        return;

    game->loadBoard(path);
}

void GameManager::update(int deltaMs)
{
    std::lock_guard<std::mutex> lock(mutex);

    for (auto &[id, game] : games)
    {
        game->checkDisconnect();
        game->update(deltaMs);

        if (game->isFinished() && !game->isRatingUpdated())
        {
            GameResult result = game->getResult();

            PlayerSession *white = game->getWhitePlayer();
            PlayerSession *black = game->getBlackPlayer();

            if (white && black)
            {
                if (result == GameResult::WhiteWin)
                {
                    white->rating = Elo::calculateWinnerRating(white->rating, black->rating);
                    black->rating = Elo::calculateLoserRating(white->rating, black->rating);

                    database.updateUserRating(white->username, white->rating);
                    database.updateUserRating(black->username, black->rating);
                }
                else if (result == GameResult::BlackWin)
                {
                    black->rating = Elo::calculateWinnerRating(black->rating, white->rating);
                    white->rating = Elo::calculateLoserRating(black->rating, white->rating);

                    database.updateUserRating(black->username, black->rating);
                    database.updateUserRating(white->username, white->rating);
                }
            }
            game->markRatingUpdated();
        }
    }
}

Game &GameManager::getGame(int gameId)
{
    return getGameImpl(*this, gameId);
}

const Game &GameManager::getGame(int gameId) const
{
    return getGameImpl(*this, gameId);
}

void GameManager::removeGame(int gameId)
{
    games.erase(gameId);
}

Game *GameManager::findGame(int gameId)
{
    return findGameImpl(*this, gameId);
}

const Game *GameManager::findGame(int gameId) const
{
    return findGameImpl(*this, gameId);
}