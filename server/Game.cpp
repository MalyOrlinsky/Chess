#include "Game.hpp"


Game::Game()
    : engine(),
      executor(engine)
{
}


void Game::executeCommand(
    const std::string& command,
    Color color)
{
    executor.execute(command, color);
}


GameSnapshot Game::snapshot() const
{
    return engine.snapshot();
}

void Game::loadBoard(const std::string& path)
{
    engine.loadBoard(path);
}


void Game::update(int deltaMs)
{
    engine.handleWait(deltaMs);
}