#include "controllerClick.hpp"

ClickResult controllerClick::onClick(CellPos pos, const Board &board, PieceStatus status,
                                     Color currentPlayer)
{
    if (!pos.valid)
    {
        if (hasSelection(currentPlayer))
            clearSelection(currentPlayer);

        return {ClickAction::None};
    }

    const Piece *piece = board.grid[pos.row][pos.col].get();

    if (!hasSelection(currentPlayer))
        return handleFirstSelection(pos, piece, status, currentPlayer);

    auto [selectedRow, selectedCol] = selected[currentPlayer];

    if (pos.row == selectedRow && pos.col == selectedCol)
        return handleClickOnSelected(pos, status, currentPlayer);

    return handleSelectionChangeOrMove(pos, board, piece, currentPlayer);
}

bool controllerClick::isLockedStatus(PieceStatus status) const
{
    return status == PieceStatus::Move ||
           status == PieceStatus::Jump ||
           status == PieceStatus::ShortReset ||
           status == PieceStatus::LongReset;
}

ClickResult controllerClick::handleFirstSelection(CellPos pos, const Piece *piece, PieceStatus status,
                                                  Color currentPlayer)
{
    if (piece == nullptr)
        return {ClickAction::None};

    if (piece->color != currentPlayer)
        return {ClickAction::None};

    if (isLockedStatus(status))
        return {ClickAction::None};

    selected[currentPlayer] = {pos.row, pos.col};

    return {ClickAction::Selected, pos, pos};
}

ClickResult controllerClick::handleClickOnSelected(CellPos pos, PieceStatus status, Color currentPlayer)
{
    if (isLockedStatus(status))
        return {ClickAction::None};

    auto [selectedRow, selectedCol] = selected[currentPlayer];

    CellPos from{selectedRow, selectedCol, true};

    clearSelection(currentPlayer);

    return {ClickAction::JumpRequest, from, from};
}

ClickResult controllerClick::handleSelectionChangeOrMove(CellPos pos, const Board &board,
                                                         const Piece *piece, Color currentPlayer)
{
    auto [selectedRow, selectedCol] = selected[currentPlayer];

    const Piece *selectedPiece = board.grid[selectedRow][selectedCol].get();

    if (piece != nullptr && piece->color == currentPlayer)
    {
        selected[currentPlayer] = {pos.row, pos.col};
        return {ClickAction::Reselected, pos, pos};
    }

    CellPos from{selectedRow, selectedCol, true};
    clearSelection(currentPlayer);

    return {ClickAction::MoveRequest, from, pos};
}

bool controllerClick::hasSelection(Color color) const
{
    auto it = selected.find(color);

    return it != selected.end() && it->second.first != -1;
}

void controllerClick::clearSelection(Color color)
{
    selected[color] = {-1, -1};
}

std::pair<int, int> controllerClick::getSelected(Color color) const
{
    auto it = selected.find(color);

    if (it == selected.end())
        return {-1, -1};

    return it->second;
}

std::map<Color, std::pair<int, int>> controllerClick::getAllSelected() const
{
    return selected;
}