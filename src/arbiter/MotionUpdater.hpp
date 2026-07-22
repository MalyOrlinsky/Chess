#pragma once

#include <vector>

#include "Motion.hpp"
#include "CollisionResolver.hpp"
#include "MotionAdvancer.hpp"
#include "../model/Board.hpp"
#include <map>

class MotionUpdater
{
public:

    std::vector<Motion> update(std::vector<Motion>& motions, std::vector<Jump>& jumps,
         Board& board, int currentClock, Color& color, std::pair<int, int>& score,
         std::map<Color, std::pair<int, int>> &selected);

private:

    bool shouldAdvance(const Motion& motion, int currentClock) const;
};