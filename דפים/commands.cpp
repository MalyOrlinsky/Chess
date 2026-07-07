#include "commands.h"
#include <iostream>
#include <sstream>
#include <cmath>

static int moveDistance(int fromRow, int fromCol, int toRow, int toCol) {
    return std::max(abs(toRow - fromRow), abs(toCol - fromCol));
}

static void applyPendingMove(GameState& state) {
    if (!state.pending.active) return;
    if (state.clock < state.pending.arrivalTime) return;

    int fr = state.pending.fromRow, fc = state.pending.fromCol;
    int tr = state.pending.toRow,   tc = state.pending.toCol;

    delete state.board.grid[tr][tc];
    state.board.grid[tr][tc] = state.board.grid[fr][fc];
    state.board.grid[fr][fc] = nullptr;
    state.pending.active = false;
}

static void handleClick(int x, int y, GameState& state) {
    int col = x / 100;
    int row = y / 100;

    if (row < 0 || row >= state.board.rows || col < 0 || col >= state.board.cols)
        return;

    Piece* cell = state.board.grid[row][col];
    bool hasSelection = state.selectedRow != -1;

    if (!hasSelection) {
        if (cell != nullptr) {
            // אם הכלי כבר בתנועה - מתעלמים
            if (state.pending.active &&
                state.pending.fromRow == row && state.pending.fromCol == col)
                return;
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
    state.pending = {state.selectedRow, state.selectedCol, row, col,
                     state.clock + dist * 1000, true};
    state.selectedRow = -1;
    state.selectedCol = -1;
}

void runCommands(const std::vector<std::string>& commands, GameState& state) {
    for (const std::string& cmd : commands) {
        if (cmd == "print board") {
            applyPendingMove(state);
            for (const auto& row : state.board.grid) {
                for (int i = 0; i < (int)row.size(); i++) {
                    if (i > 0) std::cout << " ";
                    std::cout << (row[i] ? row[i]->toString() : ".");
                }
                std::cout << "\n";
            }
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
        }
    }
}
