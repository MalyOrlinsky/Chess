#pragma once

#include <vector>

#include "Motion.hpp"
#include "CollisionResolver.hpp"
#include "MotionAdvancer.hpp"

#include "../model/Board.hpp"

class MotionUpdater
{
public:

    std::vector<Motion> update(std::vector<Motion>& motions, const std::vector<Jump>& jumps, Board& board, int currentClock);

private:

    bool shouldAdvance(const Motion& motion, int currentClock) const;
};