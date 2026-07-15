#include "RealTimeArbiter.hpp"
#include <algorithm>
#include <iostream>

RealTimeArbiter::RealTimeArbiter() {}

void RealTimeArbiter::setKingCapturedCallback(KingCapturedCallback cb) { onKingCaptured = cb; }

int RealTimeArbiter::clock() const { return currentClock; }


void RealTimeArbiter::startMotion( int fromRow, int fromCol, int toRow, int toCol, int dist) {
    motions.push_back({ fromRow, fromCol, toRow, toCol,
        currentClock, currentClock + dist, counter++});
}

void RealTimeArbiter::startJump(int row, int col, int speed) {
    jumps.push_back({row, col, currentClock, currentClock + speed, counter++});
}

bool RealTimeArbiter::isMoving(int row, int col) const {
    for (auto& m : motions)
        if ((m.fromRow == row && m.fromCol == col) || (m.toRow == row && m.toCol == col)) return true;
    return false;
}

bool RealTimeArbiter::isJumping(int row, int col) const {
    for (auto& j : jumps)
        if (j.row == row && j.col == col && currentClock < j.endTime) return true;
    return false;
}

void RealTimeArbiter::advanceClock(int ms, Board& board) {
    currentClock += ms;
    resolveArrivals(board);
    pruneExpiredJumps();
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

        int lastRow = (movingColor == Color::White) ? 0 : board.rows - 1;
        if (m.toRow == lastRow)
            board.promotePiece(m.toRow, m.toCol);


        if (kingCaptured && onKingCaptured) { onKingCaptured(movingColor); return; }

    }
}

void RealTimeArbiter::pruneExpiredJumps() {
    std::vector<Jump> active;
    for (auto& j : jumps)
        if (currentClock <= j.endTime) active.push_back(j);
    jumps = active;
}
