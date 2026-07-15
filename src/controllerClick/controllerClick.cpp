#include "controllerClick.hpp"
#include <iostream>

ClickResult controllerClick::onClick(CellPos pos, const Board& board, PieceStatus status) {
    if (!pos.valid) {
        if (hasSelection()) clearSelection();
        return {ClickAction::None};
    }

    const Piece* piece = board.grid[pos.row][pos.col].get();
std::cout << "onClick: selected=" << selectedCol << "," << selectedRow << std::endl;
    if (!hasSelection()) {
        std::cout << "onClick: no selection, pos=" << pos.row << "," << pos.col << " status=" << PieceStatusToString(status) << std::endl;
        if (piece == nullptr || status == PieceStatus::Move ||
            status == PieceStatus::Jump || status == PieceStatus::ShortReset || 
            status == PieceStatus::LongReset)
            return {ClickAction::None};
        selectedRow = pos.row;
        selectedCol = pos.col;
        return {ClickAction::Selected, pos, pos};
    }

    if (pos.row == selectedRow && pos.col == selectedCol) {
        if (status == PieceStatus::Move || status == PieceStatus::Jump || 
            status == PieceStatus::ShortReset || status == PieceStatus::LongReset)
            return {ClickAction::None};
        CellPos sel = {selectedRow, selectedCol, true};
        clearSelection();
        return {ClickAction::JumpRequest, sel, sel};
    }

    const Piece* selected = board.grid[selectedRow][selectedCol].get();
    if (piece != nullptr && selected != nullptr && piece->color == selected->color) {
        selectedRow = pos.row;
        selectedCol = pos.col;
        return {ClickAction::Reselected, pos, pos};
    }

    CellPos from = {selectedRow, selectedCol, true};
    clearSelection();
    return {ClickAction::MoveRequest, from, pos};
}

bool controllerClick::hasSelection() const { return selectedRow != -1; }
void controllerClick::clearSelection() { selectedRow = -1; selectedCol = -1; }
