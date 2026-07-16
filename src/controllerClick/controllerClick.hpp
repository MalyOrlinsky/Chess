#pragma once
#include "CellPos.hpp"
#include "ClickResult.hpp"
#include "../model/Board.hpp"
#include "../game_engine/PieceStatus.hpp"

class controllerClick {
public:
    ClickResult onClick(CellPos pos, const Board& board, PieceStatus status);
    bool hasSelection() const;
    void clearSelection();
    std::pair<int, int> getSelected() const;

private:
    int selectedRow = -1;
    int selectedCol = -1;
};
