#pragma once
#include "piece.h"

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, 'B') {}

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
                     const std::vector<std::vector<Piece*>>& grid) const override {
        if (abs(toRow - fromRow) != abs(toCol - fromCol) || toRow == fromRow) return false;

        int dr = (toRow > fromRow) ? 1 : -1;
        int dc = (toCol > fromCol) ? 1 : -1;
        int r = fromRow + dr, c = fromCol + dc;
        while (r != toRow || c != toCol) {
            if (grid[r][c] != nullptr) return false;
            r += dr;
            c += dc;
        }
        return true;
    }
};
