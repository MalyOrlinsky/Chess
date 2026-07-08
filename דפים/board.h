#pragma once
#include "piece.h"
#include "King.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "Knight.h"
#include "Pawn.h"
#include <string>
#include <vector>

struct Board {
    std::vector<std::vector<Piece*>> grid;
    int rows;
    int cols;

    ~Board() {
        for (auto& row : grid)
            for (auto* p : row)
                delete p;
    }
};

struct PendingMove {
    int fromRow, fromCol;
    int toRow, toCol;
    int arrivalTime;
    int startTime;
};

struct GameState {
    Board board;
    int selectedRow = -1;
    int selectedCol = -1;
    int clock = 0;
    std::vector<PendingMove> pending;
};

bool parseBoard(const std::vector<std::string>& lines, Board& board, std::string& error);
