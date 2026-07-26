#include "GameLoop.hpp"

GameLoop::GameLoop(GameManager& gameManager, std::function<void()> sendSnapshots) 
                    : gameManager(gameManager), sendSnapshots(sendSnapshots) {}

void GameLoop::start()
{
    std::thread(
        [this]()
        {
            while(true)
            {

                    std::cout << "GAME LOOP TICK" << std::endl;

                gameManager.update(16);

                if(sendSnapshots)
                {
                    std::cout << "GAME LOOP TICK&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << std::endl;

                    sendSnapshots();
                }    

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(16)
                );
            }

        }
    ).detach();
}