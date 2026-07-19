#include "src/game_engine/GameEngine.hpp"
#include "src/renderer/ImgRenderer.hpp"
#include <iostream>

int main()
{
    try
    {
        GameEngine engine;

        engine.loadBoard("board.txt");

        ImgRenderer renderer("assets/pieces2", "assets/board.png", engine.rows(), engine.cols());

        renderer.setCommandCallback([&](const std::string& cmd){ engine.execute(cmd); });
        renderer.setSnapCallback([&](){ return engine.snapshot(); });

        renderer.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cin.get();
    }

    return 0;
}