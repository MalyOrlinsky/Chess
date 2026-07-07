#pragma once
#include <string>
#include <vector>

struct Board {
    std::vector<std::vector<std::string>> grid;
    int rows;
    int cols;
};

bool parseBoard(const std::vector<std::string>& lines, Board& board, std::string& error);
