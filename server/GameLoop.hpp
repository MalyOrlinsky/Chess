#pragma once

#include "GameManager.hpp"
#include <thread>
#include <chrono>
#include <functional>

class GameLoop
{
public:

    GameLoop(GameManager& gameManager, std::function<void()> sendSnapshots);

    void start();

private:

    GameManager& gameManager;
    std::function<void()> sendSnapshots;

};