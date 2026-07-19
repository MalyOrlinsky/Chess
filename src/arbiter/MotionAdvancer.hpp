#pragma once

#include "../model/Board.hpp"
#include "Motion.hpp"
#include "../Constants.hpp"

class MotionAdvancer
{
public:
    std::pair<int, int> advance(Motion& motion, std::vector<Motion>& motions, const std::vector<Jump>& jumps, 
        Board& board, int currentClock, Color& color) const;
    std::pair<int, int> died(Motion& motion, Board& board, Color& color) const;
};