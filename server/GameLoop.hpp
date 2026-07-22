#pragma once

#include "GameManager.hpp"
#include <thread>
#include <chrono>

class GameLoop
{
public:

    GameLoop(GameManager& gameManager);
    
    void start();


private:

    GameManager& gameManager;
};