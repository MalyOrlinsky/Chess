#include "MotionAdvancer.hpp"

std::pair<int, int> MotionAdvancer::advance(Motion &motion, std::vector<Motion> &motions, const std::vector<Jump> &jumps,
                             Board &board, int currentClock, Color &color) const
{
    std::pair<int, int> score = {0, 0};
    if (motion.currentStep + 1 >= static_cast<int>(motion.path.size()))
        return score;

    const Position &current = motion.path[motion.currentStep];
    const Position &next = motion.path[motion.currentStep + 1];

    const Piece *piece = board.getPiece(next.row, next.col);
    if (piece)
    {
        (piece->color == Color::White ? score.first : score.second) += piece->score;

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

    return score;
}

std::pair<int, int> MotionAdvancer::died(Motion &motion, Board &board, Color &color) const
{
    std::pair<int, int> score = {0, 0};

    auto piece = board.getPiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col);
    if (piece)
    {
        if (piece->type == KING_TYPE)
            color = board.getPiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col)->color == Color::White ? Color::White : Color::Black;
        (piece->color == Color::White ? score.first : score.second) += piece->score;
    }
    board.removePiece(motion.path[motion.currentStep].row, motion.path[motion.currentStep].col);

    return score;
}
