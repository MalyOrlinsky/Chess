#include "MotionAdvancer.hpp"
#include "iostream"

std::pair<int, int> MotionAdvancer::advance(Motion &motion, std::vector<Motion> &motions, const std::vector<Jump> &jumps,
                             Board &board, int currentClock, Color &color) const
{
    std::pair<int, int> score = {0, 0};
    if (motion.currentStep + 1 >= static_cast<int>(motion.path.size()))
        return score;

    const Position &current = motion.path[motion.currentStep];
    const Position &next = motion.path[motion.currentStep + 1];

    Piece *piece = board.getPiece(next.row, next.col);
    if (piece)
    {
        SoundEffects::playCapture(piece, score, color, board, next.row, next.col);

        for (auto &m : motions)
        {
            if (m.path.size() <= m.currentStep)
                continue;

            if (m.path[m.currentStep].row == next.row || m.path[m.currentStep].col == next.col)
                m.valid = false;
        }
    }

    int lastRow = board.getPiece(current.row, current.col)->color == Color::White ? 0 : board.rows - 1;
    SoundEffects::playMove(board, current, next, lastRow);

    ++motion.currentStep;

    return score;
}

std::pair<int, int> MotionAdvancer::died(Motion &motion, Board &board, Color &color) const
{
    std::pair<int, int> score = {0, 0};

    auto piece = board.getPiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col);
    if (piece)
        SoundEffects::playCapture(piece, score, color, board, motion.path[motion.currentStep].row, motion.path[motion.currentStep].col);

    return score;
}
