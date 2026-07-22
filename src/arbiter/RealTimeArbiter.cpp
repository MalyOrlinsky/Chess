#include "RealTimeArbiter.hpp"

#include <iostream>

PieceStatus RealTimeArbiter::getNextState(const Piece &piece, PieceStatus currentState)
{
    std::string code = piece.toString();
    return animationConfig.getConfig(code, currentState).nextState;
}

std::pair<int, int> RealTimeArbiter::advanceClock(int ms, Board &board, 
                        std::map<Color, std::pair<int, int>> &selected)
{
    currentClock += ms;

    Color color = Color::None;
    std::pair<int, int> score = {0, 0};
    auto finished = motionUpdater.update(motions, jumps, board, currentClock, color, score, selected);

    if (color != Color::None)
        onKingCaptured(color);

    for (const auto &motion : finished)
    {
        const Position &pos = motion.path.back();
        enqueueNextState(PieceStatus::LongReset, pos.row, pos.col, LONG_REST_DURATION);
    }

    pruneExpiredJumps(board);
    pruneExpiredRests(board);

    return score;
}

void RealTimeArbiter::setKingCapturedCallback(KingCapturedCallback cb)
{
    onKingCaptured = cb;
}

int RealTimeArbiter::clock() const
{
    return currentClock;
}

void RealTimeArbiter::startMotion(int fromRow, int fromCol, int toRow, int toCol, int dist, Board &board)
{
    Motion motion;
    motion.path = PathBuilder::build(fromRow, fromCol, toRow, toCol, board);

    if (motion.path.size() < 2)
        return;

    motion.currentStep = 0;
    motion.startTime = currentClock;

    int steps = static_cast<int>(motion.path.size()) - 1;

    motion.stepDuration = dist / steps;
    motion.order = counter++;

    Piece *piece = board.getPiece(fromRow, fromCol);
    if (piece)
        motion.canSkip = piece->canSkip();

    motions.push_back(motion);
}

void RealTimeArbiter::startJump(int row, int col, int speed)
{
    enqueueNextState(PieceStatus::Jump, row, col, speed);
}

PieceStatus RealTimeArbiter::getStatus(int row, int col) const
{
    for (const auto &motion : motions)
    {
        if (motion.currentStep >= static_cast<int>(motion.path.size()) || !motion.valid)
            continue;

        const Position &pos = motion.path[motion.currentStep];

        if (pos.row == row && pos.col == col)
            return PieceStatus::Move;
    }

    for (const auto &j : jumps)
        if (j.row == row && j.col == col && currentClock < j.endTime)
            return PieceStatus::Jump;

    for (const auto &r : shortRests)
        if (r.row == row && r.col == col && currentClock < r.endTime)
            return PieceStatus::ShortReset;

    for (const auto &r : longRests)
        if (r.row == row && r.col == col && currentClock < r.endTime)
            return PieceStatus::LongReset;

    return PieceStatus::Idle;
}

void RealTimeArbiter::enqueueNextState(PieceStatus nextState, int row, int col, int duration)
{
    switch (nextState)
    {
    case PieceStatus::Move:
        break;

    case PieceStatus::Jump:
        jumps.push_back({row, col, currentClock, currentClock + duration, counter++});
        break;

    case PieceStatus::ShortReset:
        shortRests.push_back({row, col, currentClock, currentClock + duration, counter++});
        break;

    case PieceStatus::LongReset:
        longRests.push_back({row, col, currentClock, currentClock + duration, counter++});
        break;

    case PieceStatus::Idle:
        break;
    }
}

void RealTimeArbiter::pruneExpiredJumps(Board &board)
{
    std::vector<Jump> active;

    for (auto &j : jumps)
    {
        if (currentClock <= j.endTime)
        {
            active.push_back(j);
            continue;
        }

        Piece *piece = board.getPiece(j.row, j.col);
        if (!piece)
            continue;

        PieceStatus next = getNextState(*piece, PieceStatus::Jump);
        enqueueNextState(next, j.row, j.col, j.endTime - j.startTime);
    }

    jumps = active;
}

void RealTimeArbiter::pruneExpiredRests(Board &board)
{
    std::vector<Jump> activeShort;

    for (auto &r : shortRests)
    {
        if (currentClock <= r.endTime)
        {
            activeShort.push_back(r);
            continue;
        }

        Piece *piece = board.getPiece(r.row, r.col);
        if (!piece)
            continue;

        PieceStatus next = getNextState(*piece, PieceStatus::ShortReset);
        enqueueNextState(next, r.row, r.col, SHORT_REST_DURATION);
    }

    shortRests = activeShort;
    std::vector<Jump> activeLong;

    for (auto &r : longRests)
    {
        if (currentClock <= r.endTime)
        {
            activeLong.push_back(r);
            continue;
        }

        Piece *piece = board.getPiece(r.row, r.col);
        if (!piece)
            continue;

        PieceStatus next = getNextState(*piece, PieceStatus::LongReset);
        enqueueNextState(next, r.row, r.col, LONG_REST_DURATION);
    }

    longRests = activeLong;
}