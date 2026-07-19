// #include "BoardMapper.hpp"
// #include "../Constants.hpp"

// CellPos BoardMapper::toCell(int x, int y, int rows, int cols) const {
//     int col = x / CELL_SIZE_PX;
//     int row = y / CELL_SIZE_PX;
//     bool valid = row >= 0 && row < rows && col >= 0 && col < cols;
//     return {row, col, valid};
// }




// #include "BoardMapper.hpp"
// #include "../Constants.hpp"

// CellPos BoardMapper::toCell(int x, int y, int rows, int cols) const {
//     x -= SIDE_PANEL;
//     y -= TOP_PANEL;

//     int col = x / CELL_SIZE_PX;
//     int row = y / CELL_SIZE_PX;

//     col = col / CELL_SIZE_PX;
//     row = row / CELL_SIZE_PX;
//     bool valid = row >= 0 && row < rows && col >= 0 && col < cols;
//     return {row, col, valid};
// }





#include "BoardMapper.hpp"
#include "../Constants.hpp"

CellPos BoardMapper::toCell(int x, int y, int rows, int cols, int boardWidth, int boardHeight) const {
    x -= SIDE_PANEL;
    y -= TOP_PANEL;

    int cellWidth = boardWidth / cols;
    int cellHeight = boardHeight / rows;

    int col = x / cellWidth;
    int row = y / cellHeight;

    bool valid = row >= 0 && row < rows && col >= 0 && col < cols;

    return {row, col, valid};
}