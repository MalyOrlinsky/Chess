#include "commands.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

static int moveDistance(int fromRow, int fromCol, int toRow, int toCol) {
    return std::max(abs(toRow - fromRow), abs(toCol - fromCol));
}

static void applyPendingMoves(GameState& state) {
    std::vector<PendingMove> ready;
    std::vector<PendingMove> remaining;

    for (auto& pm : state.pending) {
        if (state.clock >= pm.arrivalTime)
            ready.push_back(pm);
        else
            remaining.push_back(pm);
    }

    // זהה התנגשויות חזיתיות - A הולך מ-X ל-Y וB הולך מ-Y ל-X
    // המנצח הוא מי שהתחיל אחרון, המפסיד נמחק
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
            if (pa->color == pb->color) continue; // ידידותיים - לא רלוונטי
            // מי שהתחיל אחרון מנצח - המפסיד נמחק ומבוטל
            if (a.startTime >= b.startTime) {
                // a מנצח - b נמחק
                delete state.board.grid[b.fromRow][b.fromCol];
                state.board.grid[b.fromRow][b.fromCol] = nullptr;
                cancelled[j] = true;
            } else {
                // b מנצח - a נמחק
                delete state.board.grid[a.fromRow][a.fromCol];
                state.board.grid[a.fromRow][a.fromCol] = nullptr;
                cancelled[i] = true;
            }
        }
    }

    // מיין לפי זמן התחלה יורד - מי שהתחיל אחרון מנצח בהתנגשות ביעד
    std::sort(ready.begin(), ready.end(),
        [](const PendingMove& a, const PendingMove& b) {
            return a.startTime > b.startTime;
        });

    std::vector<std::pair<int,int>> taken;

    for (int i = 0; i < (int)ready.size(); i++) {
        if (cancelled[i]) continue;
        auto& pm = ready[i];
        Piece* moving = state.board.grid[pm.fromRow][pm.fromCol];
        if (moving == nullptr) continue;

        bool destTaken = false;
        for (auto& t : taken)
            if (t.first == pm.toRow && t.second == pm.toCol) { destTaken = true; break; }
        if (destTaken) continue;

        Piece* target = state.board.grid[pm.toRow][pm.toCol];
        if (target != nullptr && target->color == moving->color) continue;

        delete target;
        state.board.grid[pm.toRow][pm.toCol] = moving;
        state.board.grid[pm.fromRow][pm.fromCol] = nullptr;
        taken.push_back({pm.toRow, pm.toCol});
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
