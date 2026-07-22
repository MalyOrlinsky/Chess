#pragma once
#include "AudioManager.hpp"
#include "../arbiter/Motion.hpp"
#include "../model/Board.hpp"
#include "../model/Piece.hpp"
#include "../constants.hpp"
#include <map>

namespace SoundEffects
{
    bool init();
    void playMove(Board& board, Position current, Position next, int theRow);
    void playCapture(Piece* piece, std::pair<int, int> score, Color& color, Board& board, int row, 
                int col, std::map<Color, std::pair<int, int>> &selected);
    void playPromotion(Board& board, int row, int col);
    void playGameOver(Color w, bool& gameOver, std::string winner);
    void playIllegalMove();
}