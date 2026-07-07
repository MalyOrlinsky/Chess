#pragma once
#include "piece.h"

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, 'R') {}

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
                     const std::vector<std::vector<Piece*>>& grid) const override {
        if (fromRow != toRow && fromCol != toCol) return false;

        int dr = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
        int dc = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
        int r = fromRow + dr, c = fromCol + dc;
        while (r != toRow || c != toCol) {
            if (grid[r][c] != nullptr) return false;
            r += dr;
            c += dc;
        }
        return true;
    }
};
