#include "GameLoop.hpp"

#include <thread>
#include <chrono>


GameLoop::GameLoop(GameEngine& engine) : engine(engine) {}


void GameLoop::start()
{
    std::thread(
        [this]()
        {
            while(true)
            {
                engine.handleWait(16);


                std::this_thread::sleep_for(
                    std::chrono::milliseconds(16)
                );
            }

        }
    ).detach();
}