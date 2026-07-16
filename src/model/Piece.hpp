#pragma once
#include <string>

enum class Color { White, Black, None };

class Piece {
public:
    Color color;
    char type;
    int id;

    Piece(Color c, char t);
    virtual ~Piece() = default;

    virtual Piece* onReachLastRow() const { return nullptr; }
    virtual bool canSkip() const { return false; }
    std::string toString() const;

private:
    static int nextId;
};
