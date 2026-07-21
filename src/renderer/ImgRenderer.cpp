#include "ImgRenderer.hpp"
#include <chrono>
#include "../Constants.hpp"
#include <iostream>

ImgRenderer::ImgRenderer(const std::string &spritesPath, const std::string &boardImagePath,
                         int rows, int cols)
    : loader(spritesPath), pool(loader), rows(rows), cols(cols)
{
    boardImg.read(boardImagePath, {cols * CELL_SIZE_PX, rows * CELL_SIZE_PX});
}

void ImgRenderer::setCommandCallback(CommandCallback cb)
{
    onCommand = cb;
}

void ImgRenderer::setSnapCallback(SnapCallback cb)
{
    getSnap = cb;
}

void ImgRenderer::setMyColor(Color color)
{
    myColor = color;
}

void ImgRenderer::run()
{
    Img::create_window(WINDOW_NAME);
    Img::set_mouse_callback(WINDOW_NAME, mouseHandler, this);

    auto prev = std::chrono::steady_clock::now();

    while (Img::is_window_open(WINDOW_NAME))
    {
        auto now = std::chrono::steady_clock::now();
        int dt = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count();
        prev = now;

        if (onCommand)
            onCommand("wait 0");

        GameSnapshot snap = getSnap ? getSnap() : GameSnapshot{};

        if (snap.rows == 0 || snap.cols == 0)
        {
            Img::wait_key(10);
            continue;
        }

        pool.update(snap, dt);

        Img frame = buildFrame(snap);

        frame.show_in(WINDOW_NAME);

        if (Img::wait_key(1) == 27)
            break;
    }

    Img::destroy_windows();
}

Img ImgRenderer::buildFrame(const GameSnapshot &snap)
{
    int boardWidth = boardImg.get_mat().cols;
    int boardHeight = boardImg.get_mat().rows;

    Img canvas(
        boardWidth + SIDE_PANEL * 2 + 200,
        boardHeight + TOP_PANEL + BOTTOM_PANEL);

    boardImg.draw_on(canvas, SIDE_PANEL, TOP_PANEL);

    drawTitle(canvas, snap);

    myColor = Color::None;

    if (colorCallback)
        myColor = colorCallback();

    pool.drawAll(canvas, snap, myColor);
    drawUI(canvas, snap);

    if (snap.gameOver)
        drawGameOver(canvas, snap);

    return canvas;
}

void ImgRenderer::drawUI(Img &canvas, const GameSnapshot &snap)
{
    drawPlayers(canvas, snap);
    drawMoves(canvas, snap);
}

void ImgRenderer::drawPlayers(Img &canvas, const GameSnapshot &snap)
{
    int boardWidth = cols * CELL_SIZE_PX;
    int rightPanelX = SIDE_PANEL + boardWidth + 30;

    canvas.put_text("Black", rightPanelX, TOP_PANEL + 40, 0.7);
    canvas.put_text("Score: " + std::to_string(snap.scoreBlack), rightPanelX, TOP_PANEL + 70, 0.6);
    canvas.put_text("White", rightPanelX, TOP_PANEL + 140, 0.7);
    canvas.put_text("Score: " + std::to_string(snap.scoreWhite), rightPanelX, TOP_PANEL + 170, 0.6);
}

void ImgRenderer::drawMoves(Img &canvas, const GameSnapshot &snap)
{
    int boardHeight = rows * CELL_SIZE_PX;
    canvas.put_text("Black Moves", 20, 25, 0.6);
    drawMoveList(canvas, snap.movesLogBlack, 50, TOP_PANEL + boardHeight / 2);
    canvas.put_text("White Moves", 20, TOP_PANEL + boardHeight / 2, 0.6);
    drawMoveList(canvas, snap.movesLogWhite, TOP_PANEL + boardHeight / 2 + 25, TOP_PANEL + boardHeight);
}

void ImgRenderer::drawMoveList(Img &canvas, const std::vector<std::string> &moves, int startY, int maxY)
{
    int y = startY;

    for (int i = (int)moves.size() - 1; i >= 0 && y < maxY; --i)
    {
        canvas.put_text(moves[i], 10, y, 0.45);
        y += 18;
    }
}

void ImgRenderer::drawTitle(Img &canvas, const GameSnapshot &snap)
{
    for (int c = 0; c < snap.cols; c++)
    {
        char letter = 'A' + c;

        canvas.put_text(
            std::string(1, letter),
            SIDE_PANEL + c * CELL_SIZE_PX + CELL_SIZE_PX / 2,
            TOP_PANEL - 15,
            0.6);
    }

    for (int r = 0; r < snap.rows; r++)
    {
        canvas.put_text(
            std::to_string(r + 1),
            SIDE_PANEL - 30,
            TOP_PANEL + r * CELL_SIZE_PX + CELL_SIZE_PX / 2,
            0.6);
    }
}

void ImgRenderer::drawGameOver(Img &canvas, const GameSnapshot &snap)
{
    canvas.put_text(snap.winner + " wins!", SIDE_PANEL + cols * CELL_SIZE_PX / 4,
                    TOP_PANEL + rows * CELL_SIZE_PX / 2, 2.0, cv::Scalar(0, 0, 255), 5);
}

void ImgRenderer::mouseHandler(int event, int x, int y, int, void *userdata)
{
    if (event != 1)
        return;

    auto *self = static_cast<ImgRenderer *>(userdata);
    if (self->onCommand)
        self->onCommand("click " + std::to_string(x) + " " + std::to_string(y));
}

void ImgRenderer::setColorCallback(std::function<Color()> cb)
{
    colorCallback = std::move(cb);
}