#pragma once

#include "../model/Board.hpp"
#include "Motion.hpp"

enum class CollisionResult
{
    Continue,
    Stop,
    Finish
};

class CollisionResolver
{
public:
    CollisionResult resolve(const Motion &motion, Board &board) const;
};