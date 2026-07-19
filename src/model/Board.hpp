#pragma once
#include "Piece.hpp"
#include <vector>
#include <memory>

struct Board {
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;
    int rows = 0;
    int cols = 0;

    Piece* getPiece(int row, int col) const;
    void addRow(std::vector<std::unique_ptr<Piece>> row);
    void removePiece(int row, int col);
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    void promotePiece(int row, int col);
    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) const;
};
