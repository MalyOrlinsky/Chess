#pragma once
#include "SpriteAnimator.hpp"
#include "SpriteLoader.hpp"
#include "../network/GameSnapshot.hpp"
#include "../../opencv2/src/img.hpp"
#include <map>
#include <string>
#include <set>
#include "../Constants.hpp"

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
