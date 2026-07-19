#include "MotionAdvancer.hpp"

#include <iostream>
void MotionAdvancer::advance(Motion &motion, std::vector<Motion> &motions, const std::vector<Jump> &jumps,
                             Board &board, int currentClock, Color &color) const
{
    if (motion.currentStep + 1 >= static_cast<int>(motion.path.size()))
        return;

    const Position &current = motion.path[motion.currentStep];
    const Position &next = motion.path[motion.currentStep + 1];

    const Piece *piece = board.getPiece(next.row, next.col);
    if (piece)
    {
        if (piece->type == KING_TYPE)
            color = piece->color == Color::White ? Color::White : Color::Black;

        for (auto &m : motions)
        {
            if (m.path.size() <= m.currentStep)
                continue;

            if (m.path[m.currentStep].row == next.row || m.path[m.currentStep].col == next.col)
                m.valid = false;
        }
    }

    board.movePiece(current.row, current.col, next.row, next.col);

    int lastRow = board.getPiece(next.row, next.col)->color == Color::White ? 0 : board.rows - 1;
    if (next.row == lastRow)
        board.promotePiece(next.row, next.col);

    ++motion.currentStep;
}

void MotionAdvancer::died(Motion &motion, Board &board, Color &color) const
{
    if (board.getPiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col)->type == KING_TYPE)
        color = board.getPiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col)->color == Color::White ? Color::White : Color::Black;
    board.removePiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col);
}
