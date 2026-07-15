#pragma once
#include "SpriteAnimator.hpp"
#include "SpriteLoader.hpp"
#include "GameSnapshot.hpp"
#include "../../cpp/src/img.hpp"
#include <map>
#include <string>

// Owns all SpriteAnimators, syncs them with GameSnapshot, draws them
class AnimatorPool {
public:
    explicit AnimatorPool(SpriteLoader& loader);

    // Sync animators with snapshot + advance by dt ms
    void update(const GameSnapshot& snap, int dt);

    // Draw all pieces onto canvas
    void drawAll(Img& canvas, const GameSnapshot& snap);

private:
    SpriteLoader& loader;
    std::map<std::string, SpriteAnimator> animators;

    SpriteAnimator& getOrCreate(const std::string& key,
                                const std::string& pieceCode);

    static std::string animatorKey(const CellSnapshot& cs);
    static std::string pieceKey(const CellSnapshot& cs);
};
