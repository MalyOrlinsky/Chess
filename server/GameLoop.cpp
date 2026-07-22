#include "GameLoop.hpp"

GameLoop::GameLoop(GameManager& gameManager) : gameManager(gameManager) {}

void GameLoop::start()
{
    std::thread(
        [this]()
        {
            while(true)
            {
                gameManager.update(16);

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(16)
                );
            }

        }
    ).detach();
}