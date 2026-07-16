#include "BishopRule.hpp"
#include <cmath>

bool BishopRule::canMove(int fromRow, int fromCol, int toRow, int toCol,
                         const Board& board) const {
    if (abs(toRow - fromRow) != abs(toCol - fromCol) || toRow == fromRow) return false;
    int dr = (toRow > fromRow) ? 1 : -1;
    int dc = (toCol > fromCol) ? 1 : -1;

    return true;
}
