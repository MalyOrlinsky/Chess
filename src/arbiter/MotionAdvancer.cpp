#include "MotionAdvancer.hpp"

#include <iostream>
void MotionAdvancer::advance(Motion& motion, std::vector<Motion>& motions, const std::vector<Jump>& jumps, Board& board, int currentClock) const
{
    if (motion.currentStep + 1 >= static_cast<int>(motion.path.size()))
        return;

    const Position& current = motion.path[motion.currentStep];
    const Position& next = motion.path[motion.currentStep + 1];

    const Piece* piece = board.getPiece(current.row, current.col);
    if (!piece)
    {
        for (auto j : jumps)
        {
            if (j.row == next.row && j.col == next.col && currentClock < j.endTime)
            {
                board.removePiece(current.row, current.col);
                return;
            }
        }

        std::vector<Motion> motionsActive;
        for (auto m : motions)
            if (m.path[m.currentStep].row != next.row || m.path[m.currentStep].col != next.col)
                motionsActive.push_back(m);

        motions = std::move(motionsActive);
    }

    board.movePiece(current.row, current.col, next.row, next.col);

    int lastRow = board.getPiece(next.row, next.col)->color == Color::White ? 0 : board.rows - 1;
    if (next.row == lastRow)
        board.promotePiece(next.row, next.col);

    ++motion.currentStep;
}