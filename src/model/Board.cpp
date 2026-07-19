#include "Board.hpp"
#include <iostream>

Piece* Board::getPiece(int row, int col) const {
    if (grid[row][col] == nullptr) return nullptr;
    return grid[row][col].get();
}

void Board::addRow(std::vector<std::unique_ptr<Piece>> row) {
    cols = (int)row.size();
    grid.push_back(std::move(row));
    rows = (int)grid.size();
}

void Board::removePiece(int row, int col) {
    grid[row][col].reset();
}
void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        grid[toRow][toCol].reset();
    grid[toRow][toCol] = std::move(grid[fromRow][fromCol]);
}

void Board::promotePiece(int row, int col) {
    auto piece = getPiece(row, col);
    if (!piece) return;
    Piece* promoted = piece->onReachLastRow();
    if (promoted) {
        grid[row][col].reset(promoted);
    }
}

bool Board::isPathClear(int fromRow, int fromCol, int toRow, int toCol) const {
    int rowStep = 0;
    int colStep = 0;

    if (toRow > fromRow) rowStep = 1;
    else if (toRow < fromRow) rowStep = -1;

    if (toCol > fromCol) colStep = 1;
    else if (toCol < fromCol) colStep = -1;

    bool straight = (fromRow == toRow || fromCol == toCol);
    bool diagonal = (abs(toRow - fromRow) == abs(toCol - fromCol));

    if (!straight && !diagonal)
        return true;

    int r = fromRow + rowStep;
    int c = fromCol + colStep;

    while (r != toRow || c != toCol) {
        if (grid[r][c] != nullptr)
            return false;

        r += rowStep;
        c += colStep;
    }

    return true;
}
