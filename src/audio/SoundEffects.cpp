#include "SoundEffects.hpp"
#include "iostream"

namespace
{
    AudioManager audio;
}

bool SoundEffects::init()
{
    return audio.init();
}

void SoundEffects::playMove(Board &board, Position current, Position next, int theRow)
{
    std::cout << "PlayMove" << std::endl;
    board.movePiece(current.row, current.col, next.row, next.col);

    if (next.row == theRow)
        playPromotion(board, next.row, next.col);

    audio.play("move.wav");
}

void SoundEffects::playCapture(Piece *piece, std::pair<int, int> score, Color &color, Board &board,
                               int row, int col, std::map<Color, std::pair<int, int>> &selected)
{
    std::cout << "PlayCapture#####################################################" << std::endl;
    Color pieceColor = piece->color;
    int f = pieceColor == Color::White ? 0 : pieceColor == Color::Black ? 1 : 2;
    auto it = selected.find(pieceColor);
    int r = 5555555;
    int c = 5555555;

    if (it != selected.end())
    {
        r = it->second.first;
        c = it->second.second;
    }
    std::cout << f << " " << r << " " << row << " " << c << " " << col << std::endl;
    // throw std::runtime_error("&&&&&&&&&&&&&&&&&&&&&&&");

    if (r == row && c == col)
    {
        it->second.first = -1;
        it->second.second = -1;
    }

    // selected[pieceColor] = std::make_pair(-1, -1);

    (piece->color == Color::White ? score.first : score.second) += piece->score;

    if (piece->type == KING_TYPE)
        color = piece->color == Color::White ? Color::White : Color::Black;

    board.removePiece(row, col);

    audio.play("capture.wav");
}

void SoundEffects::playPromotion(Board &board, int row, int col)
{
    board.promotePiece(row, col);
    audio.play("promotion.wav");
}

void SoundEffects::playGameOver(Color w, bool &gameOver, std::string winner)
{
    gameOver = true;
    winner = (w == Color::Black) ? "white" : "black";
    // arbiter = RealTimeArbiter();
    audio.play("game_end.wav");
}

void SoundEffects::playIllegalMove()
{
    audio.play("illegal_move.wav");
}