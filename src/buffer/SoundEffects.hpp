#pragma once
#include "../audios/AudioManager.hpp"
#include "../arbiter/Motion.hpp"
#include "../model/Board.hpp"
#include "../model/Piece.hpp"
#include "../constants.hpp"

namespace SoundEffects
{
    bool init();
    void playMove(Board& board, Position current, Position next, int theRow);
    void playCapture(Piece* piece, std::pair<int, int> score, Color& color, Board& board, int row, int col);
    void playPromotion(Board& board, int row, int col);
    void playGameOver(Color w, bool& gameOver, std::string winner);
    void playIllegalMove();
}