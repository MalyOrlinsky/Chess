#pragma once

#include "../src/game_engine/GameEngine.hpp"
#include "../src/commands/CommandExecutor.hpp"
#include "../src/network/GameSnapshot.hpp"

class Game
{
public:

    Game();

    void executeCommand(const std::string& command, Color color);

    GameSnapshot snapshot() const;

    void loadBoard(const std::string& path);

    void update(int deltaMs);

private:

    GameEngine engine;
    CommandExecutor executor;
};