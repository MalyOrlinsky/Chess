#include "Game.hpp"

Game::Game()
    : engine(),
      executor(engine)
{
}

void Game::executeCommand(
    const std::string &command,
    Color color)
{
    executor.execute(command, color);
}

GameSnapshot Game::snapshot() const
{
    GameSnapshot snapshot = engine.snapshot();

    if (status == GameStatus::Finished)
    {
        snapshot.gameOver = true;

        if (result == GameResult::WhiteWin)
            snapshot.winner = "White";
        else if (result == GameResult::BlackWin)
            snapshot.winner = "Black";
    }

    return snapshot;
}

void Game::loadBoard(const std::string &path)
{
    engine.loadBoard(path);
}

void Game::update(int deltaMs)
{
    engine.handleWait(deltaMs);
}

void Game::setPlayers(PlayerSession *white, PlayerSession *black)
{
    whitePlayer = white;
    blackPlayer = black;
}

void Game::checkDisconnect()
{
    if (status == GameStatus::Finished)
        return;

    PlayerSession *disconnected = nullptr;

    if (whitePlayer && whitePlayer->disconnected)
        disconnected = whitePlayer;
    else if (blackPlayer && blackPlayer->disconnected)
        disconnected = blackPlayer;

    if (disconnected == nullptr)
    {
        status = GameStatus::Running;
        return;
    }

    if (status != GameStatus::PlayerDisconnected)
    {
        status = GameStatus::PlayerDisconnected;

        disconnectStart = std::chrono::steady_clock::now();
        disconnectedPlayerId = disconnected->id;

        return;
    }

    auto now = std::chrono::steady_clock::now();

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - disconnectStart).count();

    if (seconds >= DISCONNECT_TIMEOUT_SECONDS)
    {
        status = GameStatus::Finished;

        if (whitePlayer && whitePlayer->id == disconnectedPlayerId)
            result = GameResult::BlackWin;
        else
            result = GameResult::WhiteWin;

        std::cout
            << "PLAYER TIMEOUT id="
            << disconnectedPlayerId
            << std::endl;
    }
}

bool Game::isFinished() const
{
    return status == GameStatus::Finished;
}

GameResult Game::getResult() const
{
    return result;
}

PlayerSession* Game::getWhitePlayer()
{
    return whitePlayer;
}

PlayerSession* Game::getBlackPlayer()
{
    return blackPlayer;
}

bool Game::isRatingUpdated() const
{
    return ratingUpdated;
}

void Game::markRatingUpdated()
{
    ratingUpdated = true;
}