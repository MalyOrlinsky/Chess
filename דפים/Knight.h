#pragma once
#include "piece.h"

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, 'N') {}

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
                     const std::vector<std::vector<Piece*>>& grid) const override {
        int dr = abs(toRow - fromRow);
        int dc = abs(toCol - fromCol);
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }
};
