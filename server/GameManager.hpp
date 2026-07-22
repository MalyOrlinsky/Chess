#pragma once

#include "Game.hpp"
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

class GameManager
{
public:
    GameManager();

    int createGame();

    void executeCommand(int gameId, const std::string& command, Color playerColor);
    GameSnapshot snapshot(int gameId) const;

    void loadBoard(int gameId, const std::string& path);
    void update(int deltaMs);

    Game& getGame(int gameId);
    const Game& getGame(int gameId) const;

    void removeGame(int gameId);

    Game* findGame(int gameId);
    const Game* findGame(int gameId) const;

private:

    template<typename Self>
    static auto findGameImpl(Self& self, int gameId);

    template<typename Self>
    static decltype(auto) getGameImpl(Self& self, int gameId);

private:

    std::map<int, std::unique_ptr<Game>> games;

    int nextGameId = 0;
};

#include "GameManager.tpp"