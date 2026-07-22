#pragma once
#include "../../opencv2/src/img.hpp"
#include "./network/GameSnapshot.hpp"
#include "AnimatorPool.hpp"
#include "SpriteLoader.hpp"
#include "../Constants.hpp"
#include <functional>
#include <string>
#include <chrono>

using CommandCallback = std::function<void(const std::string&)>;
using SnapCallback  = std::function<GameSnapshot()>;

class ImgRenderer {
public:
    ImgRenderer(const std::string& spritesPath,
                const std::string& boardImagePath,
                int rows, int cols);

    void setCommandCallback(CommandCallback cb);
    void setSnapCallback(SnapCallback cb);
    void setMyColor(Color color);

    void setColorCallback(std::function<Color()> cb);
    
    void run();

private:
    std::function<Color()> colorCallback;

    void drawPlayers(Img& canvas, const GameSnapshot& snap);
    void drawMoves(Img& canvas, const GameSnapshot& snap);
    void drawTitle(Img &canvas, const GameSnapshot &snap);
    
    void drawMoveList(Img& canvas, const std::vector<std::string>& moves, int startY, int maxY);

    static constexpr const char* WINDOW_NAME = "Kung-Fu Chess";

    SpriteLoader loader;
    AnimatorPool pool;
    Img boardImg;
    int rows, cols;
    Color myColor = Color::None;

    CommandCallback onCommand;
    SnapCallback getSnap;

    Img  buildFrame(const GameSnapshot& snap);
    void drawUI(Img& canvas, const GameSnapshot& snap);
    void drawGameOver(Img& canvas, const GameSnapshot& snap);

    static void mouseHandler(int event, int x, int y, int flags, void* userdata);
};
