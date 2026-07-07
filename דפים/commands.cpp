#include "commands.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

static int moveDistance(int fromRow, int fromCol, int toRow, int toCol) {
    return std::max(abs(toRow - fromRow), abs(toCol - fromCol));
}

static void applyPendingMoves(GameState& state) {
    // מיין לפי זמן התחלה - מי שהתחיל אחרון מנצח בהתנגשות
    std::sort(state.pending.begin(), state.pending.end(),
        [](const PendingMove& a, const PendingMove& b) {
            return a.startTime > b.startTime;
        });

    std::vector<PendingMove> remaining;
    for (auto& pm : state.pending) {
        if (state.clock < pm.arrivalTime) {
            remaining.push_back(pm);
            continue;
        }

        Piece* moving = state.board.grid[pm.fromRow][pm.fromCol];
        if (moving == nullptr) continue; // כבר הוזז

        Piece* target = state.board.grid[pm.toRow][pm.toCol];

        if (target != nullptr && target->color == moving->color) {
            // נחיתה על ידידותי - מבטלים
            continue;
        }

        // אוכל אויב או תא ריק
        delete target;
        state.board.grid[pm.toRow][pm.toCol] = moving;
        state.board.grid[pm.fromRow][pm.fromCol] = nullptr;
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
                              state.clock + dist * 1000, state.clock});
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
        } else if (cmd.substr(0, 5) == "click") {
            applyPendingMoves(state);
            int x, y;
            std::istringstream ss(cmd.substr(6));
            ss >> x >> y;
            handleClick(x, y, state);
        } else if (cmd.substr(0, 4) == "wait") {
            int ms;
            std::istringstream ss(cmd.substr(5));
            ss >> ms;
            state.clock += ms;
        }
    }
}
