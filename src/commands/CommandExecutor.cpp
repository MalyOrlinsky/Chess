#include "CommandExecutor.hpp"
#include <sstream>

CommandExecutor::CommandExecutor(GameEngine &engine) : engine(engine) {}

CommandType CommandExecutor::parseCommandType(const std::string& cmd) {
    if (cmd == "print board")        return CommandType::PrintBoard;
    if (cmd.substr(0, 5) == "click") return CommandType::Click;
    if (cmd.substr(0, 4) == "jump")  return CommandType::Jump;
    if (cmd.substr(0, 4) == "wait")  return CommandType::Wait;
    return CommandType::Unknown;
}

std::pair<int,int> CommandExecutor::parseXY(const std::string& s) {
    int x, y;
    std::istringstream(s) >> x >> y;
    return {x, y};
}

void CommandExecutor::execute(const std::string& cmd, Color playerColor) {
    std::cout << cmd << std::endl;
    switch (parseCommandType(cmd)) {
        case CommandType::PrintBoard:
            engine.handleWait(0);
            BoardPrinter().print(engine.getBoard());
            break;
        case CommandType::Click: {
            if (engine.gameOver) break;
            auto [x, y] = parseXY(cmd.substr(6));
            CellPos pos = mapper.toCell(x, y, engine.rows(), engine.cols(), 
                                        engine.cols() * CELL_SIZE_PX, engine.rows() * CELL_SIZE_PX);
            engine.handleClick(pos, playerColor);
            break;
        }
        case CommandType::Jump: {
            if (engine.gameOver) break;
            auto [x, y] = parseXY(cmd.substr(5));
            CellPos pos = mapper.toCell(x, y, engine.rows(), engine.cols(), 
                                        engine.cols() * CELL_SIZE_PX, engine.rows() * CELL_SIZE_PX);
            engine.requestJump(pos, playerColor);
            break;
        }
        case CommandType::Wait: {
            if (engine.gameOver) break;
            int ms;
            std::istringstream(cmd.substr(5)) >> ms;
            engine.handleWait(ms);
            break;
        }
        case CommandType::Unknown:
            break;
    }
}
