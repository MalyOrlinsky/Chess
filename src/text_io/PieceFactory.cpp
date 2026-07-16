#include "PieceFactory.hpp"

Piece* Pawn::onReachLastRow() const { return new Queen(color); }
bool Knight::canSkip() const { return true; }

std::unique_ptr<Piece> PieceFactory::make(const std::string& token) {
    if (token == "." || token.size() != 2) return nullptr;
    if (token[0] != 'w' && token[0] != 'b') return nullptr;
    Color c = (token[0] == 'w') ? Color::White : Color::Black;
    switch (token[1]) {
        case KING_TYPE:   return std::make_unique<King>(c);
        case ROOK_TYPE:   return std::make_unique<Rook>(c);
        case BISHOP_TYPE: return std::make_unique<Bishop>(c);
        case QUEEN_TYPE:  return std::make_unique<Queen>(c);
        case KNIGHT_TYPE: return std::make_unique<Knight>(c);
        case PAWN_TYPE:   return std::make_unique<Pawn>(c);
        default:          return nullptr;
    }
}
