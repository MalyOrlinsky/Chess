#pragma once
#include "../model/Piece.hpp"
#include "../Constants.hpp"
#include <string>
#include <memory>

class King   : public Piece { public: King(Color c)   : Piece(c, KING_TYPE)   {} };
class Rook   : public Piece { public: Rook(Color c)   : Piece(c, ROOK_TYPE)   {} };
class Bishop : public Piece { public: Bishop(Color c) : Piece(c, BISHOP_TYPE) {} };
class Queen  : public Piece { public: Queen(Color c)  : Piece(c, QUEEN_TYPE)  {} };

class Knight : public Piece { 
    public: 
        Knight(Color c) : Piece(c, KNIGHT_TYPE) {} 
        bool canSkip() const override;
};

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, PAWN_TYPE) {}
    Piece* onReachLastRow() const override;
};

class PieceFactory {
public:
    static std::unique_ptr<Piece> make(const std::string& token);
};
