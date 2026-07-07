#pragma once
#include "piece.h"
#include "pieces/King.h"
#include "pieces/Rook.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/Knight.h"
#include "pieces/Pawn.h"
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

struct GameState {
    Board board;
    int selectedRow = -1;
    int selectedCol = -1;
    int clock = 0;
};

bool parseBoard(const std::vector<std::string>& lines, Board& board, std::string& error);
