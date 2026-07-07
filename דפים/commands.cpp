#include "commands.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

static int moveDistance(int fromRow, int fromCol, int toRow, int toCol) {
    return std::max(abs(toRow - fromRow), abs(toCol - fromCol));
}

static void applyPendingMoves(GameState& state) {
    if (state.gameOver) return;

    std::vector<PendingMove> ready;
    std::vector<PendingMove> remaining;

    for (auto& pm : state.pending) {
        if (state.clock >= pm.arrivalTime)
            ready.push_back(pm);
        else
            remaining.push_back(pm);
    }

    std::vector<bool> cancelled(ready.size(), false);
    for (int i = 0; i < (int)ready.size(); i++) {
        for (int j = i + 1; j < (int)ready.size(); j++) {
            auto& a = ready[i];
            auto& b = ready[j];
            bool headOn = (a.fromRow == b.toRow && a.fromCol == b.toCol &&
                           a.toRow == b.fromRow && a.toCol == b.fromCol);
            if (!headOn) continue;
            Piece* pa = state.board.grid[a.fromRow][a.fromCol];
            Piece* pb = state.board.grid[b.fromRow][b.fromCol];
            if (pa == nullptr || pb == nullptr) continue;
            if (pa->color == pb->color) continue;
            bool aFirst = (a.startTime < b.startTime) || (a.startTime == b.startTime && a.moveOrder < b.moveOrder);
            if (!aFirst) {
                delete state.board.grid[b.fromRow][b.fromCol];
                state.board.grid[b.fromRow][b.fromCol] = nullptr;
                cancelled[j] = true;
            } else {
                delete state.board.grid[a.fromRow][a.fromCol];
                state.board.grid[a.fromRow][a.fromCol] = nullptr;
                cancelled[i] = true;
            }
        }
    }

    std::vector<std::pair<PendingMove,bool>> readyWithFlag;
    for (int i = 0; i < (int)ready.size(); i++)
        readyWithFlag.push_back({ready[i], cancelled[i]});

    std::sort(readyWithFlag.begin(), readyWithFlag.end(),
        [](const std::pair<PendingMove,bool>& a, const std::pair<PendingMove,bool>& b) {
            return a.first.startTime > b.first.startTime;
        });

    std::vector<std::pair<int,int>> taken;

    for (auto& [pm, isCancelled] : readyWithFlag) {
        if (isCancelled) continue;
        Piece* moving = state.board.grid[pm.fromRow][pm.fromCol];
        if (moving == nullptr) continue;

        bool destTaken = false;
        for (auto& t : taken)
            if (t.first == pm.toRow && t.second == pm.toCol) { destTaken = true; break; }
        if (destTaken) continue;

        Piece* target = state.board.grid[pm.toRow][pm.toCol];
        if (target != nullptr && target->color == moving->color) continue;

        bool capturedKing = (target != nullptr && target->type == 'K');
        delete target;
        state.board.grid[pm.toRow][pm.toCol] = moving;
        state.board.grid[pm.fromRow][pm.fromCol] = nullptr;
        taken.push_back({pm.toRow, pm.toCol});

        if (capturedKing) {
            state.gameOver = true;
            state.winner = (moving->color == Color::White) ? "white" : "black";
            state.pending.clear();
            state.selectedRow = -1;
            state.selectedCol = -1;
            return;
        }

        // הפיכה לווזיר
        int lastRow = (moving->color == Color::White) ? 0 : state.board.rows - 1;
        if (moving->type == 'P' && pm.toRow == lastRow) {
            Color c = moving->color;
            delete state.board.grid[pm.toRow][pm.toCol];
            state.board.grid[pm.toRow][pm.toCol] = new Queen(c);
        }
    }

    state.pending = remaining;
}

static bool isMoving(GameState& state, int row, int col) {
    for (auto& pm : state.pending)
        if (pm.fromRow == row && pm.fromCol == col)
            return true;
    return false;
}

static void handleClick(int x, int y, GameState& state) {
    if (state.gameOver) return;
    int col = x / 100;
    int row = y / 100;

    if (row < 0 || row >= state.board.rows || col < 0 || col >= state.board.cols)
        return;

    Piece* cell = state.board.grid[row][col];
    bool hasSelection = state.selectedRow != -1;

    if (!hasSelection) {
        if (cell != nullptr && !isMoving(state, row, col)) {
            state.selectedRow = row;
            state.selectedCol = col;
        }
        return;
    }

    Piece* selected = state.board.grid[state.selectedRow][state.selectedCol];
    bool sameColor = (cell != nullptr && cell->color == selected->color);

    if (sameColor) {
        state.selectedRow = row;
        state.selectedCol = col;
        return;
    }

    if (!selected->isValidMove(state.selectedRow, state.selectedCol, row, col, state.board.grid))
        return;

    int dist = moveDistance(state.selectedRow, state.selectedCol, row, col);
    state.pending.push_back({state.selectedRow, state.selectedCol, row, col,
                              state.clock + dist * 1000, state.clock, state.moveCounter++});
    state.selectedRow = -1;
    state.selectedCol = -1;
}

void runCommands(const std::vector<std::string>& commands, GameState& state) {
    for (const std::string& cmd : commands) {
        if (cmd == "print board") {
            applyPendingMoves(state);
            for (const auto& row : state.board.grid) {
                for (int i = 0; i < (int)row.size(); i++) {
                    if (i > 0) std::cout << " ";
                    std::cout << (row[i] ? row[i]->toString() : ".");
                }
                std::cout << "\n";
            }
        } else if (state.gameOver) {
            continue;
        } else if (cmd.substr(0, 5) == "click") {
            int x, y;
            std::istringstream ss(cmd.substr(6));
            ss >> x >> y;
            handleClick(x, y, state);
        } else if (cmd.substr(0, 4) == "wait") {
            int ms;
            std::istringstream ss(cmd.substr(5));
            ss >> ms;
            state.clock += ms;
            applyPendingMoves(state);
        }
    }
}
