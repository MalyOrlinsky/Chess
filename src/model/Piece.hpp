#pragma once
#include <string>

enum class Color { White, Black, None };

class Piece {
public:
    Color color;
    char type;
    int score;
    int id;

    Piece(Color c, char t, int score);
    virtual ~Piece() = default;

    virtual Piece* onReachLastRow() const { return nullptr; }
    virtual bool canSkip() const { return false; }
    virtual int dcore() const { return 0; }
    std::string toString() const;

private:
    static int nextId;
};
