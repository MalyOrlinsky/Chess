#pragma once
#include "SpriteAnimator.hpp"
#include "../config/SpriteLoader.hpp"
#include "../game_engine/GameSnapshot.hpp"
#include "../../img/src/img.hpp"
#include <map>
#include <string>

class AnimatorPool {
public:
    explicit AnimatorPool(SpriteLoader& loader);

    void update(const GameSnapshot& snap, int dt);

    void drawAll(Img& canvas, const GameSnapshot& snap, Color color);

private:
    SpriteLoader& loader;
    std::map<std::string, SpriteAnimator> animators;

    SpriteAnimator& getOrCreate(const std::string& key,
                                const std::string& pieceCode);

    static std::string animatorKey(const CellSnapshot& cs);
    static std::string pieceKey(const CellSnapshot& cs);
};
