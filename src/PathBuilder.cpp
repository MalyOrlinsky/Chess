#include "PathBuilder.hpp"
#include <cmath>


std::vector<Position> PathBuilder::build(
    int fromRow,
    int fromCol,
    int toRow,
    int toCol,
    const Board& board)
{
    std::vector<Position> path;


    const Piece* piece = board.getPiece(fromRow, fromCol);

    if (!piece)
        return path;


    path.push_back({fromRow, fromCol});


    int dr = toRow - fromRow;
    int dc = toCol - fromCol;


    //
    // Knight
    //
    if (abs(dr) == 2 && abs(dc) == 1 ||
        abs(dr) == 1 && abs(dc) == 2)
    {
        path.push_back({toRow,toCol});
        return path;
    }


    //
    // ישר או אלכסון
    //
    int rowStep = 0;
    int colStep = 0;


    if (dr > 0) rowStep = 1;
    if (dr < 0) rowStep = -1;

    if (dc > 0) colStep = 1;
    if (dc < 0) colStep = -1;


    int r = fromRow + rowStep;
    int c = fromCol + colStep;


    while (r != toRow || c != toCol)
    {
        path.push_back({r,c});

        r += rowStep;
        c += colStep;
    }


    path.push_back({toRow,toCol});


    return path;
}