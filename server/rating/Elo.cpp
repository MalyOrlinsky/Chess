#include "Elo.hpp"
#include <cmath>

int Elo::calculateWinnerRating(int winnerRating, int loserRating)
{
    double expected =  1.0 / (1.0 + std::pow(10.0, (loserRating - winnerRating) / 400.0));
    int change =  static_cast<int>(32 * (1 - expected));

    return winnerRating + change;
}


int Elo::calculateLoserRating(int winnerRating, int loserRating)
{
    double expected =
        1.0 / (1.0 + std::pow(10.0, (winnerRating - loserRating) / 400.0));

    int change =
        static_cast<int>(32 * expected);

    return loserRating - change;
}