#include "Piece.hpp"

int Piece::nextId = 0;

Piece::Piece(Color c, char t, int score) : color(c), type(t), id(nextId++), score(score) {}

std::string Piece::toString() const {
    std::string s;
    s += (color == Color::White ? 'w' : 'b');
    s += type;
    return s;
}
