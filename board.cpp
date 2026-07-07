#include "board.h"
#include <sstream>
#include <set>

static const std::set<std::string> VALID_TOKENS = {
    "wK","bK","wQ","bQ","wR","bR","wN","bN","wB","bB","wP","bP","."
};

bool parseBoard(const std::vector<std::string>& lines, Board& board, std::string& error) {
    int rowWidth = -1;
    for (const std::string& line : lines) {
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (ss >> token) {
            if (VALID_TOKENS.find(token) == VALID_TOKENS.end()) {
                error = "ERROR UNKNOWN_TOKEN";
                return false;
            }
            tokens.push_back(token);
        }
        
        if (rowWidth == -1) 
            rowWidth = tokens.size();
        else if ((int)tokens.size() != rowWidth) {
            error = "ERROR ROW_WIDTH_MISMATCH";
            return false;
        }
        board.grid.push_back(tokens);
    }
    board.rows = board.grid.size();
    board.cols = rowWidth == -1 ? 0 : rowWidth;
    return true;
}
