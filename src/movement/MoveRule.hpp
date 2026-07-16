#pragma once
#include "../model/Board.hpp"
#include "../game_engine/PieceStatus.hpp"

class MoveRule {
public:
    virtual ~MoveRule() = default;
    virtual bool canMove(int fromRow, int fromCol, int toRow, int toCol,
                         const Board& board) const = 0;
    RealTimeArbiter arbiter = RealTimeArbiter("assets/pieces2");
};
