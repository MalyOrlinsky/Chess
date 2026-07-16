#pragma once

#include "../model/Board.hpp"
#include "Motion.hpp"

class MotionAdvancer
{
public:
    void advance(Motion& motion, std::vector<Motion>& motions, const std::vector<Jump>& jumps, Board& board, int currentClock) const;
};