#pragma once
#include <string>
#include "../game_engine/GameEngine.hpp"
#include "../text_io/BoardPrinter.hpp"
#include "CommandType.hpp"

class CommandExecutor
{
public:
    explicit CommandExecutor(GameEngine &engine);

    void execute(const std::string &command, Color playerColor);

private:
    CommandType parseCommandType(const std::string &cmd);
    std::pair<int, int> parseXY(const std::string &s);

    GameEngine &engine;
    BoardMapper mapper;
};