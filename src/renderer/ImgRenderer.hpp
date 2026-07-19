#pragma once
#include "../../cpp/src/img.hpp"
#include "GameSnapshot.hpp"
#include "AnimatorPool.hpp"
#include "../config/SpriteLoader.hpp"
#include "../controllerClick/CellPos.hpp"
#include "../Constants.hpp"
#include <functional>
#include <string>

using CommandCallback = std::function<void(const std::string&)>;
using SnapCallback  = std::function<GameSnapshot()>;

class ImgRenderer {
public:
    ImgRenderer(const std::string& spritesPath,
                const std::string& boardImagePath,
                int rows, int cols);

    void setCommandCallback(CommandCallback cb);
    void setSnapCallback(SnapCallback cb);
    
    void run();

private:
    void drawPlayers(Img& canvas, const GameSnapshot& snap);
    void drawMoves(Img& canvas, const GameSnapshot& snap);
    void drawTitle(Img &canvas, const GameSnapshot &snap);
    
    void drawMoveList(Img& canvas, const std::vector<std::string>& moves, int startY, int maxY);

    static constexpr const char* WINDOW_NAME = "Kung-Fu Chess";

    SpriteLoader loader;
    AnimatorPool pool;
    Img boardImg;
    int rows, cols;

    CommandCallback onCommand;
    SnapCallback getSnap;

    Img  buildFrame(const GameSnapshot& snap);
    void drawUI(Img& canvas, const GameSnapshot& snap);
    void drawGameOver(Img& canvas, const GameSnapshot& snap);

    static void mouseHandler(int event, int x, int y, int flags, void* userdata);
};
