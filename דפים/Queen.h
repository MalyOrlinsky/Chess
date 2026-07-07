#pragma once
#include "piece.h"

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, 'Q') {}

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
                     const std::vector<std::vector<Piece*>>& grid) const override {
        int dr = abs(toRow - fromRow);
        int dc = abs(toCol - fromCol);
        bool diagonal = dr == dc && dr > 0;
        bool straight = (fromRow == toRow) != (fromCol == toCol);
        if (!diagonal && !straight) return false;

        int stepR = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
        int stepC = (toCol > fromCol) ? 1 : (toCol < fromCol) ? -1 : 0;
        int r = fromRow + stepR, c = fromCol + stepC;
        while (r != toRow || c != toCol) {
            if (grid[r][c] != nullptr) return false;
            r += stepR;
            c += stepC;
        }
        return true;
    }
};
