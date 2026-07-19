#pragma once

#include "../model/Board.hpp"
#include "../renderer/GameSnapshot.hpp"
#include "../arbiter/RealTimeArbiter.hpp"
#include "../controllerClick/controllerClick.hpp"

class SnapshotBuilder
{
public:

    GameSnapshot build(
        const Board& board,
        const RealTimeArbiter& arbiter,
        const controllerClick& controller,
        bool gameOver,
        const std::string& winner,
        const std::pair<int,int>& score,
        const std::vector<std::string>& movesLogWhite,
        const std::vector<std::string>& movesLogBlack
    ) const;
};