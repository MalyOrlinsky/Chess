#include <set>
#include "AnimatorPool.hpp"
#include "../Constants.hpp"

AnimatorPool::AnimatorPool(SpriteLoader &loader) : loader(loader) {}

void AnimatorPool::update(const GameSnapshot &snap, int dt)
{
    std::set<std::string> alive;

    for (int r = 0; r < snap.rows; r++)
    {
        for (int c = 0; c < snap.cols; c++)
        {
            const CellSnapshot &cs = snap.cells[r][c];
            if (cs.type == 0)
                continue;

            std::string key = animatorKey(cs);
            alive.insert(key);

            SpriteAnimator &anim = getOrCreate(key, pieceKey(cs));
            // if (anim.getState() != cs.status)
            // {
            //     std::cout
            //         << "STATE CHANGE id="
            //         << cs.id
            //         << " "
            //         << PieceStatusToString(anim.getState())
            //         << " -> "
            //         << PieceStatusToString(cs.status)
            //         << std::endl;
            // }
            // if (anim.getState() != cs.status)
            //     anim.setState(cs.status);
            anim.tick(dt);
        }
    }

    for (auto it = animators.begin(); it != animators.end();)
    {
        if (!alive.count(it->first))
            it = animators.erase(it);
        else
            ++it;
    }
}

void AnimatorPool::drawAll(Img &canvas, const GameSnapshot &snap)
{
    for (int r = 0; r < snap.rows; r++)
    {
        for (int c = 0; c < snap.cols; c++)
        {
            const CellSnapshot &cs = snap.cells[r][c];
            if (cs.type == 0)
                continue;
            if (cs.status == PieceStatus::Jump)
                std::cout << "DRAWING type=" << cs.type << " color=" << (cs.color == Color::White ? 'w' : 'b') << " status=" << PieceStatusToString(cs.status) << std::endl;
            std::string key = animatorKey(cs);
            animators.at(key).setState(cs.status);
            const Img &frame = animators.at(key).currentImg();
            const_cast<Img &>(frame).draw_on(canvas,
                                             c * CELL_SIZE_PX, r * CELL_SIZE_PX);
        }
    }
}

SpriteAnimator &AnimatorPool::getOrCreate(const std::string &key,
                                          const std::string &pieceCode)
{
    if (!animators.count(key))
        animators.emplace(key, SpriteAnimator(pieceCode, loader));
    return animators.at(key);
}

std::string AnimatorPool::animatorKey(const CellSnapshot &cs)
{
    return std::to_string(cs.id);
}

std::string AnimatorPool::pieceKey(const CellSnapshot &cs)
{
    std::string code;
    code += (cs.color == Color::White) ? 'w' : 'b';
    code += cs.type;
    return code;
}
