#include "ImgRenderer.hpp"
#include <chrono>

ImgRenderer::ImgRenderer(const std::string& spritesPath,
                         const std::string& boardImagePath,
                         int rows, int cols)
    : loader(spritesPath), pool(loader), rows(rows), cols(cols) {
    boardImg.read(boardImagePath, {cols * CELL_SIZE_PX, rows * CELL_SIZE_PX});
}

void ImgRenderer::setClickCallback(ClickCallback cb) { onClick = cb; }
void ImgRenderer::setWaitCallback(WaitCallback cb)   { onWait  = cb; }
void ImgRenderer::setSnapCallback(SnapCallback cb)   { getSnap = cb; }

void ImgRenderer::run() {
    Img::create_window(WINDOW_NAME);
    Img::set_mouse_callback(WINDOW_NAME, mouseHandler, this);

    auto prev = std::chrono::steady_clock::now();
    while (Img::is_window_open(WINDOW_NAME)) {
        auto now = std::chrono::steady_clock::now();
        int dt = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count();
        prev = now;

        if (onWait) onWait(dt);

        GameSnapshot snap = getSnap ? getSnap() : GameSnapshot{};
        pool.update(snap, dt);

        Img frame = buildFrame(snap);
        frame.show_in(WINDOW_NAME);

        if (Img::wait_key(1) == 27) break;
    }
    Img::destroy_windows();
}

Img ImgRenderer::buildFrame(const GameSnapshot& snap) {
    Img canvas = boardImg;
    pool.drawAll(canvas, snap);
    drawUI(canvas, snap);
    if (snap.gameOver)
        drawGameOver(canvas, snap);
    return canvas;
}

void ImgRenderer::drawUI(Img& canvas, const GameSnapshot& snap) {
    int y1 = rows * CELL_SIZE_PX - 40;
    int y2 = rows * CELL_SIZE_PX - 15;
    canvas.put_text(snap.playerWhite + "  " + std::to_string(snap.scoreWhite), 5, y1, 0.6);
    canvas.put_text(snap.playerBlack + "  " + std::to_string(snap.scoreBlack), 5, y2, 0.6);

    for (int i = 0, shown = (int)snap.movesLog.size() - 1;
         i < 5 && shown >= 0; i++, shown--)
        canvas.put_text(snap.movesLog[shown],
                        cols * CELL_SIZE_PX - 120, 20 + i * 22, 0.45);
}

void ImgRenderer::drawGameOver(Img& canvas, const GameSnapshot& snap) {
    canvas.put_text(snap.winner + " wins!",
                    cols * CELL_SIZE_PX / 4,
                    rows * CELL_SIZE_PX / 2,
                    2.0);
}


CellPos ImgRenderer::pixelToCell(int x, int y) const {
    int col = x / CELL_SIZE_PX;
    int row = y / CELL_SIZE_PX;
    bool valid = row >= 0 && row < rows && col >= 0 && col < cols;
    return {row, col, valid};
}

void ImgRenderer::mouseHandler(int event, int x, int y, int, void* userdata) {
    if (event != 1) return;
    auto* self = static_cast<ImgRenderer*>(userdata);
    if (!self->onClick) return;
    self->onClick(self->pixelToCell(x, y));
}
