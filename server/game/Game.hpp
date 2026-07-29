#pragma once

#include "../../src/game_engine/GameEngine.hpp"
#include "../../src/commands/CommandExecutor.hpp"
#include "../../src/network/GameSnapshot.hpp"
#include "../PlayerSession.hpp"

#include <chrono>

enum class GameStatus
{
    Running,
    PlayerDisconnected,
    Finished
};

enum class GameResult
{
    None,
    WhiteWin,
    BlackWin
};

class Game
{
public:
    Game();

    void executeCommand(const std::string &command, Color color);

    GameSnapshot snapshot() const;

    void loadBoard(const std::string &path);

    void update(int deltaMs);

    void setPlayers(PlayerSession *white, PlayerSession *black);

    void checkDisconnect();

    bool isFinished() const;

    GameResult getResult() const;

    PlayerSession* getWhitePlayer();
    PlayerSession* getBlackPlayer();

    bool isRatingUpdated() const;
    void markRatingUpdated();

private:
    GameEngine engine;
    CommandExecutor executor;

    PlayerSession *whitePlayer = nullptr;
    PlayerSession *blackPlayer = nullptr;

    GameResult result = GameResult::None;
    GameStatus status = GameStatus::Running;
    bool ratingUpdated = false;

    std::chrono::steady_clock::time_point disconnectStart;
    int disconnectedPlayerId = -1;

    static constexpr int DISCONNECT_TIMEOUT_SECONDS = 30;
};