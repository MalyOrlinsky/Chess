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
        std::cout << "piece " << piece->type << " died" << std::endl;
        if (piece->type == KING_TYPE)
            color = piece->color == Color::White ? Color::White : Color::Black;
        std::vector<Motion> motionsActive;

        for (auto &m : motions)
        {
         std::cout << "1 " << std::endl;
           if (m.path.size() <= m.currentStep)
                continue;

            if (m.path[m.currentStep].row != next.row || m.path[m.currentStep].col != next.col)
                motionsActive.push_back(m);
        }
         std::cout << "2" << std::endl;

        motions = motionsActive;
    }
         std::cout << "3" << std::endl;

    board.movePiece(current.row, current.col, next.row, next.col);
         std::cout << "12 " << std::endl;

    int lastRow = board.getPiece(next.row, next.col)->color == Color::White ? 0 : board.rows - 1;
         std::cout << "4 " << std::endl;
    if (next.row == lastRow)
        board.promotePiece(next.row, next.col);
         std::cout << "5" << std::endl;

    ++motion.currentStep;
}

void MotionAdvancer::died(Motion &motion, Board &board, Color &color) const
{
    if (board.getPiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col)->type == KING_TYPE)
        color = board.getPiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col)->color == Color::White ? Color::White : Color::Black;
    board.removePiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col);
}
