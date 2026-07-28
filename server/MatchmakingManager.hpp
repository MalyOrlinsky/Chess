#pragma once

#include "PlayerSession.hpp"
#include "GameManager.hpp"

#include <vector>
#include <functional>

class MatchmakingManager
{
public:
    using MatchFoundCallback = std::function<void(PlayerSession &, PlayerSession &)>;

    MatchmakingManager(GameManager &gameManager, MatchFoundCallback callback);

    void addPlayer(PlayerSession *player);
    void startMatch(PlayerSession &player1, PlayerSession &player2);
    
private:
    void tryMatch();
    void assignPlayers(PlayerSession &white, PlayerSession &black, int gameId);
    int createGame();
    void createMatch(PlayerSession &white, PlayerSession &black);

private:
    GameManager &gameManager;

    MatchFoundCallback matchFoundCallback;

    std::vector<PlayerSession*> queue;
};