#pragma once

#include "../src/game_engine/GameEngine.hpp"


class GameLoop
{
public:

    GameLoop(GameEngine& engine);

    void start();


private:

    GameEngine& engine;
};