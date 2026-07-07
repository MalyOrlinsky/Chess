#pragma once
#include "../piece.h"

class King : public Piece {
public:
    King(Color c) : Piece(c, 'K') {}

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
                     const std::vector<std::vector<Piece*>>& grid) const override {
        int dr = abs(toRow - fromRow);
        int dc = abs(toCol - fromCol);
        return dr <= 1 && dc <= 1 && (dr + dc > 0);
    }
};
