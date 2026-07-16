#pragma once
#include "../model/Piece.hpp"
#include "../game_engine/PieceStatus.hpp"
#include <vector>
#include <string>

struct CellSnapshot {
    int id = -1;
    char type = 0;
    Color color;
    PieceStatus status = PieceStatus::Idle;
};
struct GameSnapshot {
    int rows = 0;
    int cols = 0;
    std::vector<std::vector<CellSnapshot>> cells;
    bool gameOver = false;
    std::string winner;

    std::string playerWhite;
    std::string playerBlack;
    int scoreWhite = 0;
    int scoreBlack = 0;
    std::vector<std::string> movesLog;
};
