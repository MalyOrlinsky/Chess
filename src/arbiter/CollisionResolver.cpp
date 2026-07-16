#include "CollisionResolver.hpp"

CollisionResult CollisionResolver::resolve(const Motion& motion, Board& board) const {
    if (motion.currentStep + 1 >= static_cast<int>(motion.path.size()))
        return CollisionResult::Finish;

    const Position& current = motion.path[motion.currentStep];
    const Position& next = motion.path[motion.currentStep + 1];

    Piece* moving = board.getPiece(current.row, current.col);

    if (moving == nullptr)
        return CollisionResult::Stop;

    Piece* target = board.getPiece(next.row, next.col);

    if (target == nullptr)
        return CollisionResult::Continue;

    // if (motion.canSkip)
    //     return CollisionResult::Continue;

    if (target->color != moving->color)
        return CollisionResult::Finish;

    return CollisionResult::Stop;
}