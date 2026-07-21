#pragma once
#include "../model/Board.hpp"
#include "../rule_engine/RuleEngine.hpp"
#include "../arbiter/Motion.hpp"
#include "../arbiter/RealTimeArbiter.hpp"
#include "../controllerClick/controllerClick.hpp"
#include "../controllerClick/BoardMapper.hpp"
#include "GameSnapshot.hpp"
#include "../config/AnimationConfigProvider.hpp"
#include "../config/AnimConfig.hpp"
#include <string>
#include <vector>
#include "../text_io/BoardParser.hpp"
#include <fstream>
#include <iostream>
#include "SnapshotBuilder.hpp"
#include "../buffer/SoundEffects.hpp"

class GameEngine
{
public:
    std::pair<int, int> score;
    bool gameOver = false;
    std::string winner;

    int rows() const;
    int cols() const;

    GameEngine();

    void handleClick(CellPos pos, Color color);
    void requestMove(CellPos from, CellPos to);
    void requestJump(CellPos pos, Color playerColor);
    void handleWait(int ms);
    int clock() const;
    GameSnapshot snapshot() const;

    void loadBoard(const std::string &path);
    Board &getBoard();

private:
    std::string cellToNotation(int row, int col);

    void recordAction(Piece *piece, const std::string &action);

    Board board;

    RuleEngine ruleEngine;
    RealTimeArbiter arbiter;
    controllerClick controller;
    AnimationConfigProvider animationConfig;
    SnapshotBuilder snapshotBuilder;

    std::vector<std::string> movesLogWhite;
    std::vector<std::string> movesLogBlack;
};
