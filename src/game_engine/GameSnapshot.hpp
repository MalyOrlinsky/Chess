#pragma once
#include "../model/Piece.hpp"
#include "PieceStatus.hpp"
#include "../arbiter/Motion.hpp"
#include <vector>
#include <string>
#include <map>

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

    std::map<Color, std::pair<int, int>> selected;

    std::vector<Motion> motions;

    std::string playerWhite;
    std::string playerBlack;
    int scoreWhite = 0;
    int scoreBlack = 0;
    std::vector<std::string> movesLogBlack;
    std::vector<std::string> movesLogWhite;
};
