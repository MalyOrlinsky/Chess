#pragma once

#include "../model/Board.hpp"
#include "../arbiter/Motion.hpp"


class PathBuilder
{
public:

    static std::vector<Position> build(
        int fromRow,
        int fromCol,
        int toRow,
        int toCol,
        const Board& board
    );

};