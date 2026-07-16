#pragma once

#include "../model/Board.hpp"
#include "Motion.hpp"

enum class CollisionResult
{
    Continue,
    Stop,
    Finish,
    Died
};

class CollisionResolver
{
public:
    CollisionResult resolve(const Motion &motion, std::vector<Jump> jumps, Board &board) const;
};