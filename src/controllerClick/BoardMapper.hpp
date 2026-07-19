#pragma once

#include "CellPos.hpp"

class BoardMapper
{
public:
    CellPos toCell(int x, int y, int rows, int cols, int boardWidth, int boardHeight) const;
};