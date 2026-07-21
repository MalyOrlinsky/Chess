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

void SoundEffects::playMove(Board& board, Position current, Position next, int theRow)
{
   std::cout << "PlayMove" << std::endl;
    board.movePiece(current.row, current.col, next.row, next.col);

    if (next.row == theRow)
        playPromotion(board, next.row, next.col);

    audio.play("move.wav");
}

void SoundEffects::playCapture(Piece* piece, std::pair<int, int> score, Color& color, Board& board, int row, int col)
{
   std::cout << "PlayCapture" << std::endl;

    (piece->color == Color::White ? score.first : score.second) += piece->score;

    if (piece->type == KING_TYPE)
        color = piece->color == Color::White ? Color::White : Color::Black;

    board.removePiece(row, col);

    audio.play("capture.wav");
}

void SoundEffects::playPromotion(Board& board, int row, int col)
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