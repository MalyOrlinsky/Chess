#pragma once

#include <vector>

#include "Motion.hpp"
#include "CollisionResolver.hpp"
#include "MotionAdvancer.hpp"

#include "../model/Board.hpp"

class MotionUpdater
{
public:

    std::vector<Motion> update(std::vector<Motion>& motions, const std::vector<Jump>& jumps,
         Board& board, int currentClock, Color& color, std::pair<int, int>& score);

private:

    bool shouldAdvance(const Motion& motion, int currentClock) const;
};