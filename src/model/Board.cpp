#include "Board.hpp"

Piece* Board::getPiece(int row, int col) const {
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

char Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    char capturedType = 0;
    if (grid[toRow][toCol])
        capturedType = grid[toRow][toCol]->type;
    grid[toRow][toCol] = std::move(grid[fromRow][fromCol]);
    return capturedType;
}

void Board::promotePiece(int row, int col) {
    Piece* promoted = grid[row][col]->onReachLastRow();
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
        return false;

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