#pragma once
#include "../model/Piece.hpp"
#include "../Constants.hpp"
#include <string>
#include <memory>

class King   : public Piece { public: King(Color c)   : Piece(c, KING_TYPE, 0)   {} };
class Rook   : public Piece { public: Rook(Color c)   : Piece(c, ROOK_TYPE, 5)   {} };
class Bishop : public Piece { public: Bishop(Color c) : Piece(c, BISHOP_TYPE, 7) {} };
class Queen  : public Piece { public: Queen(Color c)  : Piece(c, QUEEN_TYPE, 9)  {} };

class Knight : public Piece { 
    public: 
        Knight(Color c) : Piece(c, KNIGHT_TYPE, 5) {} 
        bool canSkip() const override;
};

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, PAWN_TYPE, 3) {}
    Piece* onReachLastRow() const override;
};

class PieceFactory {
public:
    static std::unique_ptr<Piece> make(const std::string& token);
};
