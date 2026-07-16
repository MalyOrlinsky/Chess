#pragma once
#include "Motion.hpp"
#include "../game_engine/PieceStatus.hpp"
#include "../model/Board.hpp"
#include "../Constants.hpp"
#include "../config/SpriteLoader.hpp"
#include <vector>
#include <functional>

class RealTimeArbiter {
public:

    explicit RealTimeArbiter(const std::string& spritePath);
    
    using KingCapturedCallback = std::function<void(Color winner)>;

    void setKingCapturedCallback(KingCapturedCallback cb);
    int clock() const;
    void startMotion(int fromRow, int fromCol, int toRow, int toCol, int dist);
    void startJump(int row, int col, int speed);
    PieceStatus getStatus(int row, int col) const;
    void advanceClock(int ms, Board& board);
    void enqueueNextState(PieceStatus nextState, int row, int col, int duration);
    PieceStatus getNextState(const Piece& piece, PieceStatus currentState);

private:
    int currentClock = 0;
    int counter = 0;
    std::vector<Motion> motions;
    std::vector<Jump> jumps;
    std::vector<Jump> shortRests;
    std::vector<Jump> longRests;
    KingCapturedCallback onKingCaptured;

    SpriteLoader spriteLoader;

    void resolveArrivals(Board& board);
    void resolveHeadOn(std::vector<Motion>& ready, std::vector<bool>& cancelled, Board& board);
    void resolveJumpCaptures(std::vector<Motion>& ready, std::vector<bool>& cancelled, Board& board);
    void applyArrivals(std::vector<Motion>& ready, std::vector<bool>& cancelled, Board& board);
    void pruneExpiredJumps(Board& board);
    void pruneExpiredRests(Board& board);
};
