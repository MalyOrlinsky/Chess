#include "RealTimeArbiter.hpp"
#include <algorithm>
#include <iostream>

RealTimeArbiter::RealTimeArbiter(const std::string& spritePath)
    : spriteLoader(spritePath)
{
}

PieceStatus RealTimeArbiter::getNextState(const Piece& piece,
                                          PieceStatus currentState)
{
    std::string code = piece.toString();

    return spriteLoader.configManager.getConfig(code, currentState).nextState;
}

void RealTimeArbiter::advanceClock(int ms, Board& board)
{
    currentClock += ms;

    resolveArrivals(board);
    pruneExpiredJumps(board);
    pruneExpiredRests(board);
}

void RealTimeArbiter::pruneExpiredJumps(Board& board)
{
    std::vector<Jump> active;

    for (auto& j : jumps)
    {
        if (currentClock <= j.endTime)
        {
            std::cout << "jump active: row=" << j.row << " col=" << j.col << " startTime=" << j.startTime << " endTime=" << j.endTime << std::endl;
            active.push_back(j);
            continue;
        }

        Piece* piece = board.getPiece(j.row, j.col);

        if (!piece)
            continue;

        PieceStatus next = getNextState(*piece, PieceStatus::Jump);
        std::cout << "piece=" << piece->toString() << " from Jump to " << PieceStatusToString(next) << std::endl;

        enqueueNextState(next, j.row, j.col, j.endTime - j.startTime);
    }

    jumps = active;
}

void RealTimeArbiter::pruneExpiredRests(Board& board)
{
    std::vector<Jump> activeShort;

    for (auto& r : shortRests)
    {
        if (currentClock <= r.endTime)
        {
            activeShort.push_back(r);
            continue;
        }

        Piece* piece = board.getPiece(r.row, r.col);

        if (!piece)
            continue;

        PieceStatus next =  getNextState(*piece, PieceStatus::ShortReset);

        enqueueNextState(next, r.row, r.col, SHORT_REST_DURATION);
    }

    shortRests = activeShort;

    std::vector<Jump> activeLong;

    for (auto& r : longRests)
    {
        if (currentClock <= r.endTime)
        {
            activeLong.push_back(r);
            continue;
        }

        Piece* piece = board.getPiece(r.row, r.col);

        if (!piece)
            continue;

        PieceStatus next = getNextState(*piece, PieceStatus::LongReset);

        enqueueNextState(next, r.row, r.col, LONG_REST_DURATION);
    }

    longRests = activeLong;
}

void RealTimeArbiter::setKingCapturedCallback(KingCapturedCallback cb) { onKingCaptured = cb; }

int RealTimeArbiter::clock() const { return currentClock; }


void RealTimeArbiter::startMotion( int fromRow, int fromCol, int toRow, int toCol, int dist) {
    motions.push_back({ fromRow, fromCol, toRow, toCol,
        currentClock, currentClock + dist, counter++});
}

void RealTimeArbiter::startJump(int row, int col, int speed) {
    enqueueNextState(PieceStatus::Jump, row, col, speed);
}

PieceStatus RealTimeArbiter::getStatus(int row, int col) const {
    for (auto& m : motions)
        if (m.fromRow == row && m.fromCol == col)
            return PieceStatus::Move;
    for (auto& j : jumps)
        if (j.row == row && j.col == col && currentClock < j.endTime)
            return PieceStatus::Jump;
    for (auto& r : shortRests)
        if (r.row == row && r.col == col && currentClock < r.endTime)
            return PieceStatus::ShortReset;
    for (auto& r : longRests)
        if (r.row == row && r.col == col && currentClock < r.endTime)
            return PieceStatus::LongReset;
    return PieceStatus::Idle;
}

void RealTimeArbiter::resolveArrivals(Board& board) {
    std::vector<Motion> ready, remaining;
    for (auto& m : motions)
        (m.arrivalTime <= currentClock ? ready : remaining).push_back(m);
    motions = remaining;

    if (ready.empty()) return;

    std::vector<bool> cancelled(ready.size(), false);
    resolveHeadOn(ready, cancelled, board);
    resolveJumpCaptures(ready, cancelled, board);
    applyArrivals(ready, cancelled, board);
}

void RealTimeArbiter::resolveHeadOn(std::vector<Motion>& ready, std::vector<bool>& cancelled, Board& board) {
    for (int i = 0; i < (int)ready.size(); i++) {
        for (int j = i + 1; j < (int)ready.size(); j++) {
            auto& a = ready[i]; auto& b = ready[j];
            if (a.toRow != b.fromRow || a.toCol != b.fromCol) continue;
            if (b.toRow != a.fromRow || b.toCol != a.fromCol) continue;

            Piece* pa = board.getPiece(a.fromRow, a.fromCol);
            Piece* pb = board.getPiece(b.fromRow, b.fromCol);
            if (!pa || !pb || pa->color == pb->color) continue;

            bool aFirst = a.startTime < b.startTime ||
                          (a.startTime == b.startTime && a.order < b.order);
            int loser = aFirst ? j : i;
            auto& lm = ready[loser];
            board.removePiece(lm.fromRow, lm.fromCol);
            cancelled[loser] = true;
        }
    }
}

void RealTimeArbiter::resolveJumpCaptures(std::vector<Motion>& ready, std::vector<bool>& cancelled, Board& board) {
    for (int i = 0; i < (int)ready.size(); i++) {
        if (cancelled[i]) continue;
        auto& m = ready[i];
        Piece* moving = board.getPiece(m.fromRow, m.fromCol);
        if (!moving) continue;

        for (auto& j : jumps) {
            if (j.row != m.toRow || j.col != m.toCol) continue;
            if (m.arrivalTime < j.startTime || m.arrivalTime > j.endTime) continue;
            Piece* jumper = board.getPiece(j.row, j.col);
            if (!jumper || jumper->color == moving->color) continue;

            bool kingCaptured = (moving->type == KING_TYPE);
            Color winnerColor = jumper->color;
            board.removePiece(m.fromRow, m.fromCol);
            cancelled[i] = true;
            if (kingCaptured && onKingCaptured) onKingCaptured(winnerColor);
            break;
        }
    }
}

void RealTimeArbiter::applyArrivals(std::vector<Motion>& ready, std::vector<bool>& cancelled, Board& board) {
    std::vector<std::pair<Motion, bool>> withFlag;
    for (int i = 0; i < (int)ready.size(); i++)
        withFlag.push_back({ready[i], cancelled[i]});

    std::sort(withFlag.begin(), withFlag.end(),
        [](const auto& a, const auto& b) {
            if (a.first.startTime != b.first.startTime)
                return a.first.startTime < b.first.startTime;
            return a.first.order < b.first.order;
        });

    for (auto& [m, isCancelled] : withFlag) {
        if (isCancelled) continue;
        Piece* moving = board.getPiece(m.fromRow, m.fromCol);
        if (!moving) continue;

        Piece* target = board.getPiece(m.toRow, m.toCol);
        if (target && target->color == moving->color) continue;

        bool kingCaptured = (target && target->type == KING_TYPE);
        Color movingColor = moving->color;

        board.movePiece(m.fromRow, m.fromCol, m.toRow, m.toCol);
        PieceStatus next = getNextState(*board.getPiece(m.toRow, m.toCol), PieceStatus::Move);
        enqueueNextState(next, m.toRow, m.toCol, LONG_REST_DURATION);

        int lastRow = (movingColor == Color::White) ? 0 : board.rows - 1;
        if (m.toRow == lastRow)
            board.promotePiece(m.toRow, m.toCol);


        if (kingCaptured && onKingCaptured) { onKingCaptured(movingColor); return; }

    }
}

void RealTimeArbiter::enqueueNextState(PieceStatus nextState, int row, int col, int duration)
{
    std::cout << "enqueueNextState: nextState=" << PieceStatusToString(nextState) << " row=" << row << " col=" << col << " duration=" << duration << std::endl;
    switch (nextState)
    {
        case PieceStatus::Move:
            break;
        case PieceStatus::Jump:
            jumps.push_back({row, col, currentClock, currentClock + duration,
                counter++});
            break;

        case PieceStatus::ShortReset:
            shortRests.push_back({row, col, currentClock,
                                  currentClock + duration, counter++});
            break;

        case PieceStatus::LongReset:
            longRests.push_back({row, col, currentClock, currentClock + duration,
                                 counter++});
            break;

        case PieceStatus::Idle:
            break;
    }
}
