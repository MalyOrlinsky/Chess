#include "SnapshotBuilder.hpp"


GameSnapshot SnapshotBuilder::build(
    const Board& board,
    const RealTimeArbiter& arbiter,
    const controllerClick& controller,
    bool gameOver,
    const std::string& winner,
    const std::pair<int,int>& score,
    const std::vector<std::string>& movesLogWhite,
    const std::vector<std::string>& movesLogBlack
) const
{
    GameSnapshot snap;

    snap.rows = board.rows;
    snap.cols = board.cols;

    auto selected = controller.getSelected();

    snap.selectedRow = selected.first;
    snap.selectedCol = selected.second;

    snap.gameOver = gameOver;
    snap.winner = winner;

    snap.scoreWhite = score.second;
    snap.scoreBlack = score.first;

    snap.playerWhite = "white";
    snap.playerBlack = "black";

    snap.movesLogWhite = movesLogWhite;
    snap.movesLogBlack = movesLogBlack;

    snap.cells.resize(board.rows, std::vector<CellSnapshot>(board.cols));

    for(int r = 0; r < board.rows; r++)
    {
        for(int c = 0; c < board.cols; c++)
        {
            Piece* piece = board.getPiece(r,c);
            if(!piece) continue;

            snap.cells[r][c] = {
                piece->id,
                piece->type,
                piece->color,
                arbiter.getStatus(r,c)
            };
        }
    }

    return snap;
}