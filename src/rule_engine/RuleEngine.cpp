#include "RuleEngine.hpp"
#include "../movement/RookRule.hpp"
#include "../movement/BishopRule.hpp"
#include "../movement/QueenRule.hpp"
#include "../movement/KnightRule.hpp"
#include "../movement/KingRule.hpp"
#include "../movement/PawnRule.hpp"
#include "../Constants.hpp"

RuleResult RuleEngine::validate(int fromRow, int fromCol, int toRow, int toCol,
                                const Board& board) const {
    if (!inBounds(fromRow, fromCol, board) || !inBounds(toRow, toCol, board))
        return RuleResult::OutsideBoard;

    const Piece* piece = board.getPiece(fromRow, fromCol);
    if (piece == nullptr)
        return RuleResult::EmptySource;

    const Piece* target = board.getPiece(toRow, toCol);
    if (target != nullptr && target->color == piece->color)
        return RuleResult::FriendlyDestination;

    auto rule = ruleFor(piece->type);
    if (rule == nullptr || !rule->canMove(fromRow, fromCol, toRow, toCol, board))
        return RuleResult::IllegalPieceMove;

    if (!board.isPathClear(fromRow, fromCol, toRow, toCol))
        return RuleResult::BlockingTool;

    return RuleResult::Ok;
}

bool RuleEngine::inBounds(int row, int col, const Board& board) const {
    return row >= 0 && row < board.rows && col >= 0 && col < board.cols;
}

std::unique_ptr<MoveRule> RuleEngine::ruleFor(char type) const {
    switch (type) {
        case ROOK_TYPE:   return std::make_unique<RookRule>();
        case BISHOP_TYPE: return std::make_unique<BishopRule>();
        case QUEEN_TYPE:  return std::make_unique<QueenRule>();
        case KNIGHT_TYPE: return std::make_unique<KnightRule>();
        case KING_TYPE:   return std::make_unique<KingRule>();
        case PAWN_TYPE:   return std::make_unique<PawnRule>();
        default:          return nullptr;
    }
}