#pragma once

#include "Motion.hpp"
#include "MotionUpdater.hpp"
#include "../game_engine/PieceStatus.hpp"
#include "../model/Board.hpp"
#include "../config/SpriteLoader.hpp"
#include "../movement/PathBuilder.hpp"
#include "../Constants.hpp"
#include <vector>
#include <functional>
#include <string>

class RealTimeArbiter
{
public:

    explicit RealTimeArbiter(const std::string& spritePath);

    using KingCapturedCallback = std::function<void(Color winner)>;

    void setKingCapturedCallback(KingCapturedCallback cb);

    int clock() const;

    void startMotion(int fromRow, int fromCol, int toRow, int toCol, int dist, Board& board);
    void startJump(int row, int col, int speed);

    PieceStatus getStatus(int row, int col) const;

    std::pair<int, int> advanceClock(int ms, Board& board);

private:

    void pruneExpiredJumps(Board& board);
    void pruneExpiredRests(Board& board);

    PieceStatus getNextState(const Piece& piece, PieceStatus currentState);
    void enqueueNextState(PieceStatus nextState, int row, int col, int duration);

private:

    int currentClock = 0;
    int counter = 0;

    std::vector<Motion> motions;
    std::vector<Jump> jumps;
    std::vector<Jump> shortRests;
    std::vector<Jump> longRests;

    KingCapturedCallback onKingCaptured;
    SpriteLoader spriteLoader;
    MotionUpdater motionUpdater;
};