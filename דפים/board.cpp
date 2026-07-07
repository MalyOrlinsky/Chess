#include "board.h"
#include <sstream>

static Piece* makePiece(const std::string& token) {
    if (token == ".") return nullptr;
    if (token.size() != 2) return nullptr;

    Color c = (token[0] == 'w') ? Color::White : Color::Black;
    if (token[0] != 'w' && token[0] != 'b') return nullptr;

    switch (token[1]) {
        case 'K': return new King(c);
        case 'R': return new Rook(c);
        case 'B': return new Bishop(c);
        case 'Q': return new Queen(c);
        case 'N': return new Knight(c);
        case 'P': return new Pawn(c);
        default:  return nullptr;
    }
}

bool parseBoard(const std::vector<std::string>& lines, Board& board, std::string& error) {
    int rowWidth = -1;
    for (const std::string& line : lines) {
        std::istringstream ss(line);
        std::string token;
        std::vector<Piece*> row;
        while (ss >> token) {
            Piece* p = makePiece(token);
            if (token != "." && p == nullptr) {
                for (auto* piece : row) delete piece;
                error = "ERROR UNKNOWN_TOKEN";
                return false;
            }
            row.push_back(p);
        }
        if (rowWidth == -1) rowWidth = row.size();
        else if ((int)row.size() != rowWidth) {
            for (auto* piece : row) delete piece;
            error = "ERROR ROW_WIDTH_MISMATCH";
            return false;
        }
        board.grid.push_back(row);
    }
    board.rows = board.grid.size();
    board.cols = rowWidth == -1 ? 0 : rowWidth;
    return true;
}
