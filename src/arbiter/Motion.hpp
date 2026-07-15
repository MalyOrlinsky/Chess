#pragma once
#include <string>

enum class PieceStatus { Idle, ShortReset, LongReset, Move, Jump };

inline std::string PieceStatusToString(PieceStatus state)
{
    switch (state)
    {
        case PieceStatus::Idle:       return "idle";
        case PieceStatus::ShortReset: return "short_rest";
        case PieceStatus::LongReset:  return "long_rest";
        case PieceStatus::Move:       return "move";
        case PieceStatus::Jump:       return "jump";
    }
    return "idle";
}

inline PieceStatus StringToPieceStatus(const std::string& state)
{
    if (state == "idle")
        return PieceStatus::Idle;

    if (state == "short_rest")
        return PieceStatus::ShortReset;

    if (state == "long_rest")
        return PieceStatus::LongReset;

    if (state == "move")
        return PieceStatus::Move;

    if (state == "jump")
        return PieceStatus::Jump;

    return PieceStatus::Idle;
}

struct Motion {
    int fromRow, fromCol;
    int toRow, toCol;
    int startTime;
    int arrivalTime;
    int order;
};

struct Jump {
    int row, col;
    int startTime;
    int endTime;
    int order;
};
