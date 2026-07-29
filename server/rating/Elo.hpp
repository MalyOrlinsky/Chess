#pragma once

class Elo
{
public:
    static int calculateWinnerRating(int winnerRating, int loserRating);

    static int calculateLoserRating(int winnerRating, int loserRating);
};