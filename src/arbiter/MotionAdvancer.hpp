#pragma once

#include "../model/Board.hpp"
#include "Motion.hpp"
#include "../Constants.hpp"

class MotionAdvancer
{
public:
    void advance(Motion& motion, std::vector<Motion>& motions, const std::vector<Jump>& jumps, 
        Board& board, int currentClock, Color& color) const;
    void died(Motion& motion, Board& board, Color& color) const;
};