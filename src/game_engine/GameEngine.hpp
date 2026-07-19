#pragma once
#include "../model/Board.hpp"
#include "../rule_engine/RuleEngine.hpp"
#include "../arbiter/Motion.hpp"
#include "../arbiter/RealTimeArbiter.hpp"
#include "../controllerClick/controllerClick.hpp"
#include "../controllerClick/BoardMapper.hpp"
#include "../renderer/GameSnapshot.hpp"
#include "../config/SpriteLoader.hpp"
#include "../config/AnimConfig.hpp"
#include "CommandType.hpp"
#include <string>
#include <vector>
#include "../text_io/BoardParser.hpp"
#include <fstream>
#include "SnapshotBuilder.hpp"

class GameEngine {
public:
    std::pair<int, int> score;
    bool gameOver = false;
    std::string winner;

    int rows() const;
    int cols() const;

    GameEngine();

    void handleClick(CellPos pos);
    void requestMove(CellPos from, CellPos to);
    void requestJump(CellPos pos);
    void handleWait(int ms);
    void execute(const std::string& cmd);
    int clock() const;
    GameSnapshot snapshot() const;

    void loadBoard(const std::string& path);
    Board& getBoard();


private:

    std::string cellToNotation(int row, int col);

    void recordAction(Piece* piece, const std::string& action);

    Board board;

    BoardMapper mapper;
    RuleEngine ruleEngine;
    RealTimeArbiter arbiter;
    controllerClick controller;
    SpriteLoader spriteLoader;
    SnapshotBuilder snapshotBuilder;

    std::vector<std::string> movesLogWhite;
    std::vector<std::string> movesLogBlack;
};
