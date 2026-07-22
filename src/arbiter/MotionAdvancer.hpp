#pragma once
#include "../audio/SoundEffects.hpp"
#include "../model/Board.hpp"
#include "Motion.hpp"
#include "../Constants.hpp"
#include <map>

class MotionAdvancer
{
public:

    MotionAdvancer(std::map<Color, std::pair<int, int>>& selected);

    std::pair<int, int> advance(Motion& motion, std::vector<Motion>& motions, const std::vector<Jump>& jumps, 
        Board& board, int currentClock, Color& color);
    std::pair<int, int> died(Motion& motion, Board& board, Color& color);

private:

    std::map<Color, std::pair<int, int>> selected;
};