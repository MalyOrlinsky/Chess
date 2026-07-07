#pragma once
#include "../piece.h"

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, 'P') {}

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol,
                     const std::vector<std::vector<Piece*>>& grid) const override {
        int dir = (color == Color::White) ? -1 : 1; // לבן עולה, שחור יורד
        int startRow = (color == Color::White) ? (int)grid.size() - 2 : 1;

        int dr = toRow - fromRow;
        int dc = toCol - fromCol;

        // צעד אחד קדימה לתא ריק
        if (dc == 0 && dr == dir && grid[toRow][toCol] == nullptr)
            return true;

        // צעד שתיים קדימה מהשורה ההתחלתית
        if (dc == 0 && dr == 2 * dir && fromRow == startRow
            && grid[fromRow + dir][fromCol] == nullptr
            && grid[toRow][toCol] == nullptr)
            return true;

        // אכילה אלכסונית
        if (abs(dc) == 1 && dr == dir && grid[toRow][toCol] != nullptr
            && grid[toRow][toCol]->color != color)
            return true;

        return false;
    }
};
