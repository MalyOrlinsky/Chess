#pragma once
#include "../model/Board.hpp"
#include "../rule_engine/RuleEngine.hpp"
#include "../arbiter/RealTimeArbiter.hpp"
#include "../controllerClick/controllerClick.hpp"
#include "../controllerClick/BoardMapper.hpp"
#include "../renderer/GameSnapshot.hpp"
#include "../renderer/SpriteLoader.hpp"
#include "CommandType.hpp"
#include <string>

class GameEngine {
public:
    Board board;
    bool gameOver = false;
    std::string winner;

    GameEngine();
    void handleClick(CellPos pos);
    void requestMove(CellPos from, CellPos to);
    void requestJump(CellPos pos);
    void handleWait(int ms);
    void execute(const std::string& cmd);
    int clock() const;
    GameSnapshot snapshot() const;

    BoardMapper mapper;

private:

    std::string pieceCode(const Piece& p);

    RuleEngine ruleEngine;
    RealTimeArbiter arbiter;
    controllerClick controller;
    SpriteLoader spriteLoader;
};