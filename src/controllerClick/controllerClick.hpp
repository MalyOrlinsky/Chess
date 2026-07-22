#pragma once

#include <map>
#include "CellPos.hpp"
#include "ClickResult.hpp"
#include "../model/Board.hpp"
#include "../game_engine/PieceStatus.hpp"
#include "../model/Piece.hpp"

class controllerClick {
public:
    ClickResult onClick(CellPos pos, const Board& board, PieceStatus status, Color currentPlayer);

    bool hasSelection(Color color) const;
    void clearSelection(Color color);
    std::pair<int, int> getSelected(Color color) const;
    std::map<Color, std::pair<int, int>> getAllSelected() const;

private:
    std::map<Color, std::pair<int, int>> selected;

    bool isLockedStatus(PieceStatus status) const;

    ClickResult handleFirstSelection(CellPos pos, const Piece* piece, PieceStatus status, 
                                    Color currentPlayer);
    ClickResult handleClickOnSelected(CellPos pos, PieceStatus status, Color currentPlayer);
    ClickResult handleSelectionChangeOrMove(CellPos pos, const Board& board, const Piece* piece, 
                                            Color currentPlayer);
};